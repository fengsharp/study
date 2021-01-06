#include <assert.h>
#include <stdio.h>

#include <functional>

#include <unistd.h>

#include "Channel.h"
#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Types.h"

#include <sys/timerfd.h>

int createTimerfd()
{
    int ret = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(ret >= 0);
    return ret;
}

void setTimer(int timerfd, Timestamp when)
{
    int64_t microseconds = when.microSeconds() - Timestamp::now().microSeconds();
    if (microseconds < 100)
    {
        microseconds = 100;
    }

    struct itimerspec itspec;
    memZero(&itspec, sizeof(itspec));
    itspec.it_value.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSeconds);
    itspec.it_value.tv_nsec = static_cast<long>(microseconds % Timestamp::kMicroSecondsPerSeconds * 1000);
    int ret = timerfd_settime(timerfd, 0, &itspec, NULL);
    assert(ret == 0);
}

void readTimer(int timerfd)
{
    int64_t howmany = 0;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    assert(n == sizeof(howmany));
}

EventLoop * g_pLoop = NULL;

void timerFunc(Timestamp receivedTime)
{
    puts("--- timer func callback. ---");

    g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    int timerfd = createTimerfd();
    Channel timerChannel(g_pLoop, timerfd);
    timerChannel.setEventReadCallback(std::bind(&timerFunc, std::placeholders::_1));
    timerChannel.enableReading();

    Timestamp when = addTime(Timestamp::now(), 2);
    setTimer(timerfd, when);

    loop.loop();

    puts("=== ok. ===");

    return 0;
}