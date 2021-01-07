#include <assert.h>
#include <stdio.h>

#include <functional>
#include <string>

#include <unistd.h>

#include "Channel.h"
#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Types.h"
#include "EventLoopThread.h"

EventLoop * g_pLoop = NULL;

void timerCallbak(const std::string & msg)
{
    static int count = 0;
    ++count;

    printf("--- msg: %s, timer call back. %d--\n", msg.data(), count);

    if (count > 2)
    {
        puts("### quit ###");
        g_pLoop->stop();
    }
}

void normalCallback(const std::string & msg)
{
    printf("--- !msg: %s, timer call back. --\n", msg.data());
    g_pLoop->runEvery(1, std::bind(&timerCallbak, "runEvery"));
}

int main()
{
    EventLoopThread loopThread;
    g_pLoop = loopThread.startLoop();

    g_pLoop->runAfter(3, std::bind(&normalCallback, "runAfter1"));
    CurrentThread::sleepUsec(6 * 1000 * 1000);

    puts("=== ok. ===");

    return 0;
}