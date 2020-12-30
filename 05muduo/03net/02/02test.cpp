#include <memory>
#include <stdio.h>
#include <strings.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"

EventLoop * g_loop = nullptr;
int timerfd = 0;

void timeoutFun(Timestamp receiveTime)
{
    printf("===Timeout\n");
    uint64_t howmany;
    ::read(timerfd, &howmany, sizeof(howmany));
    g_loop->quit();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel timerChannl(&loop, timerfd);
    timerChannl.enableReading();
    timerChannl.setReadCallback(std::bind(timeoutFun, std::placeholders::_1));

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 1;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);

    return 0;
}