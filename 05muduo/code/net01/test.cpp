#include <functional>
#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"

EventLoop * g_loop = NULL;

void timeoutFun(Timestamp receiveTime)
{
    printf("Timeout\n");
    g_loop->quit();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel chanel(g_loop, timerfd);
    chanel.setReadCallback(std::bind(timeoutFun, std::placeholders::_1));
    chanel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 2;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();
    ::close(timerfd);
    
    puts("=== over. ===");

    return 0;
}