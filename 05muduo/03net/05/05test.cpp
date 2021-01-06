#include <assert.h>
#include <stdio.h>

#include <functional>

#include <unistd.h>

#include "Channel.h"
#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Types.h"




EventLoop * g_pLoop = NULL;

void timerCallbak()
{
    puts("--- timer call back. ---");
    g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    loop.runAfter(2, std::bind(&timerCallbak));
    loop.loop();

    puts("=== ok. ===");

    return 0;
}