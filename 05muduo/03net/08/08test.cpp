#include <stdio.h>

#include "EventLoop.h"

int main()
{
    EventLoop loop;
    loop.loop();

    puts("=== ok. ===");
    return 0;
}