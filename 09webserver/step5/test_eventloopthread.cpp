#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "EventLoop.h"
#include "EventLoopThread.h"

void runInThread()
{
    printf("runInThread(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
}

int main()
{
    printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

    EventLoopThread loopThread;
    EventLoop * loop = loopThread.startLoop();
    // 异步调用runInThread，即将runInThread添加到loop对象所在IO线程，让该IO线程执行
    loop->runInLoop(runInThread);
    sleep(1);
    // runAfter内部也调用了runInLoop，所以这里也是异步调用
    loop->runAfter(2, runInThread);
    sleep(3);
    loop->quit();

    sleep(1);
    printf("exit main().\n");

    return 0;
}