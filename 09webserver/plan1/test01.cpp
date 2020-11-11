#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Thread.h"

void threadFunc()
{
    printf("threadFunc(): pid=%d tid=%d\n", getpid(), CurrentThread::gettid());

    EventLoop loop;
    loop.loop();
}

int main()
{
    printf("main(): pid=%d tid=%d\n", getpid(), CurrentThread::gettid());
    
    Thread t(std::bind(threadFunc));
    t.start();

    EventLoop loop;
    loop.loop();

    t.join();

    return 0;
}