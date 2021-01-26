#include <stdio.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Types.h"

EventLoop * g_loop;

void timeout()
{
    printf("Timeout!\n");
    g_loop->stop();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    loop.runAfter(5, std::bind(&timeout));

    loop.loop();

    puts("=== ok. ===");
}