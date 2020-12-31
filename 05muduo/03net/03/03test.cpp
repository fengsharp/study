#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/timerfd.h>

#include <functional>

#include "Channel.h"
#include "EventLoop.h"

EventLoop * g_pLoop;

void timerCallback(Timestamp time)
{
    puts("--- bebebe ---");
    
    g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(fd > 0);

    Channel timerChannel(g_pLoop, fd);
    timerChannel.enableReading();
    timerChannel.setReadEventFunc(std::bind(&timerCallback, std::placeholders::_1));

    struct itimerspec howlong;
    memset(&howlong, 0, sizeof(howlong));
    howlong.it_value.tv_sec = 2;
    timerfd_settime(fd, 0, &howlong, NULL);

    loop.loop();

    puts("=== ok. ===");

    return 0;
}