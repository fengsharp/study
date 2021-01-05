#include <stdio.h>

#include <assert.h>
#include <sys/timerfd.h>

#include <functional>

#include "Channel.h"
#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"
#include "Types.h"

EventLoop * g_pLoop = NULL;

void readCallback(Timestamp receivedTime)
{
    puts("--- time out. ---");
    g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    int timefd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(timefd > 0);
    Channel timeChannel(g_pLoop, timefd);
    timeChannel.setReadCallback(std::bind(&readCallback, std::placeholders::_1));
    timeChannel.enableRead();

    struct itimerspec when;
    memZero(&when, sizeof(when));
    when.it_value.tv_sec = 2;

    int ret = timerfd_settime(timefd, 0, &when, NULL);
    assert(ret == 0);

    loop.loop();

    puts("=== ok. ===");

    return 0;
}