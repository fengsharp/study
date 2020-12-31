#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/timerfd.h>

#include <functional>

#include "Channel.h"
#include "EventLoop.h"

EventLoop * g_pLoop;

int g_cnt = 0;
void timerCallback()
{
    printf("--- bebebe --- %d\n", g_cnt);

    ++g_cnt;
    if (g_cnt > 5)
    {
        g_pLoop->stop();
    }
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;
    puts("start...");
    loop.runAfter(3.0f, std::bind(&timerCallback));
    loop.runAfter(1.0f, std::bind(&timerCallback));
    loop.runEvery(2.0f, std::bind(&timerCallback));
    loop.loop();

    puts("=== ok. ===");

    return 0;
}