#include <stdio.h>

#include "EventLoop.h"
#include "Thread.h"

void func1()
{
    EventLoop loop;
    loop.loop();
}

int main()
{
    Thread t1(std::bind(func1), "t1");
    t1.start();

    EventLoop loop;
    loop.loop();

    t1.join();

    puts("=== over. ===");

    return 0;
}