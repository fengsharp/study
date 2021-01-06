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

EventLoop * g_pLoop = NULL;

void timerCallbak(const std::string & msg)
{
    static int count = 0;
    ++count;

    printf("--- msg: %s, timer call back. %d--\n", msg.data(), count);

    if (count > 10)
    {
        g_pLoop->stop();
    }
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    loop.runAfter(2, std::bind(&timerCallbak, "runAfter1"));
    loop.runAfter(2, std::bind(&timerCallbak, "runAfter2"));
    loop.runAfter(2, std::bind(&timerCallbak, "runAfter3"));
    loop.runEvery(1, std::bind(&timerCallbak, "runEvery"));
    loop.loop();

    puts("=== ok. ===");

    return 0;
}