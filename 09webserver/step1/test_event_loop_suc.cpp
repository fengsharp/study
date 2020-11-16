#include "EventLoop.h"

#include "Thread.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void threadFunc()
{
    printf("threadFunc(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

    EventLoop loop;
    loop.loop();
}

int main()
{
    printf("main pid=%d, tid=%d\n", getpid(), CurrentThread::gettid());

    EventLoop loop;
    Thread t(std::bind(threadFunc));
    t.start();

    loop.loop();
    t.join();

    return 0;
}