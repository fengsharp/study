#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "EventLoop.h"
#include "EventLoopThread.h"

void runInThread()
{
    printf("runInThread: pid=%d tid=%d\n", getpid(), CurrentThread::tid());
}

int main()
{
    printf("main: pid=%d tid=%d\n", getpid(), CurrentThread::tid());

    EvnetLoopThread loopThread;
    EventLoop * pLoop = loopThread.startLoop();
    pLoop->runInLoop(runInThread);
    sleep(1);
    pLoop->runAfter(2, runInThread);
    sleep(3);
    pLoop->quit();

    puts("=== over. ===");

    return 0;
}