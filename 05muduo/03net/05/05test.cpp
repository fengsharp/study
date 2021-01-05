#include <stdio.h>

#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"

EventLoop * g_pLoop = NULL;

void waitStop()
{
    CurrentThread::sleepUsec(3000);
    g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    Thread t1(std::bind(&waitStop), "t1");
    t1.start();

    g_pLoop->loop();

    t1.join();

    puts("=== ok. ===");

    return 0;
}