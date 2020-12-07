#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <functional>

#include "EventLoop.h"
#include "Thread.h"

void threadFunc()
{
    printf("thread func pid = %d, tid=%d\n", getpid(), CurrentThread::tid());
    EventLoop loop;
    loop.loop();
}

int main()
{
    printf("thread func pid = %d, tid=%d\n", getpid(), CurrentThread::tid());
    Thread t1(std::bind(threadFunc), "thread1");
    t1.start();

    EventLoop loop;
    loop.loop();

    t1.join();

    return 0;
}