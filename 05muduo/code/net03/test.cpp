#include <functional>
#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "EventLoop.h"


EventLoop * g_loop = NULL;
int g_flag = 0;

void printFlag(const char * msg)
{
    printf("%s pid = %d, flag = %d\n", msg, getpid(), g_flag);
}

void run4()
{
    printFlag("run4");
    g_loop->quit();
}

void run3()
{
    printFlag("run3");
    g_loop->runAfter(3, std::bind(run4));
    g_flag = 3;
}

void run2()
{
    printFlag("run2");
    g_loop->queueInLoop(std::bind(run3));
}

void run1()
{
    g_flag = 1;
    printFlag("run1");
    g_loop->runInLoop(std::bind(run2));
    g_flag = 2;
}

int main()
{
    printFlag("main()");
    EventLoop loop;
    g_loop = &loop;

    loop.runAfter(2, std::bind(run1));
    loop.loop();

    printFlag("main()");

    puts("=== over. ===");

    return 0;
}