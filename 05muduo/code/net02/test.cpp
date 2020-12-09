#include <functional>
#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "EventLoop.h"


EventLoop * g_loop = NULL;
int cnt = 0;

void printTid()
{
    printf("pid=%d, tid=%d\n", getpid(), CurrentThread::gettid());
    printf("now %s\n", Timestamp::now().toString().data());
}

void print(const char * msg)
{
    printf("msg %s %s\n", Timestamp::now().toString().data(), msg);
    if (++cnt >= 20)
    {
        g_loop->quit();
    }
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    printTid();
    printf("main\n");

    loop.runAfter(1, std::bind(print, "once1"));
    loop.runAfter(1.5, std::bind(print, "once1.5"));
    loop.runAfter(2.5, std::bind(print, "once2.5"));
    loop.runAfter(3.5, std::bind(print, "once3.5"));

    loop.runEvery(2, std::bind(print, "every2"));
    loop.runEvery(3, std::bind(print, "every3"));

    loop.loop();

    puts("=== over. ===");

    return 0;
}