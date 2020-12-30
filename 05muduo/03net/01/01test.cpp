#include <stdio.h>

#include "EventLoop.h"

int main()
{
    EventLoop loop;
    puts("start...");
    loop.loop();
    puts("end...");

    return 0;
}