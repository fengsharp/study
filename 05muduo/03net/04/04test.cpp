#include <stdio.h>

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <functional>

#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "Timestamp.h"
#include "Types.h"

void runInThread()
{
    printf("runInThread pid=%d, tid=%d\n", getpid(), CurrentThread::tid());
}

int main()
{
    printf("main pid=%d, tid=%d\n", getpid(), CurrentThread::tid());

    EventLoopThread loopThread;
    EventLoop * pLoop = loopThread.startLoop();
    pLoop->runInLoop(std::bind(&runInThread));
    pLoop->stop();

    puts("=== ok. ===");

    return 0;
}