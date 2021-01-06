#include <stdio.h>

#include <functional>

#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"

EventLoop * g_pLoop = NULL;

void threadFunc()
{
    CurrentThread::sleepUsec(3000);
    g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    Thread t1(std::bind(&threadFunc), "t1");
    t1.start();

    loop.loop();

    t1.join();

    puts("=== ok. ===");

    return 0;
}