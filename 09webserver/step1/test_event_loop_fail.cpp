#include <stdio.h>

#include "EventLoop.h"
#include <Thread.h>

EventLoop * g_loop = nullptr;

void threadFunc()
{
    g_loop->loop();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    Thread t(std::bind(threadFunc));
    t.start();
    t.join();

    return 0;
}