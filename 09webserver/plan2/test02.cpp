#include <functional>
#include <stdio.h>
#include <strings.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Thread.h"

EventLoop * g_loop;
int timefd;

void time_out(Timestamp receiveTime)
{
    printf("Timeout\n");
    uint64_t howmany;
    ::read(timefd, &howmany, sizeof(howmany));
    g_loop->quit();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    timefd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(g_loop, timefd);
    channel.setReadCallback(std::bind(time_out, std::placeholders::_1));
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 1;
    ::timerfd_settime(timefd, 0, &howlong, NULL);

    loop.loop();

    ::close(timefd);

    return 0;
}