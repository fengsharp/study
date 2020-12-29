#include <stdio.h>
#include <unistd.h>

#include <string>

#include "CountDownLatch.h"
#include "CurrentThread.h"
#include "ThreadPool.h"

void print()
{
    printf("print tid=%d\n", CurrentThread::tid());
}

void printString(const std::string & str)
{
    printf("printString tid=%d %s\n", CurrentThread::tid(), str.data());
    usleep(100);
}

int main()
{
    ThreadPool pool;
    pool.start(5);

    pool.addTask(std::bind(&print));
    pool.addTask(std::bind(&print));

    const int testNum = 20;
    for (int i = 0; i < testNum; i++)
    {
        char buf[32] = { 0 };
        snprintf(buf, sizeof(buf) - 1, "hello-%d", i);
        pool.addTask(std::bind(&printString, std::string(buf)));
    }

    CountDownLatch latch(1);
    pool.addTask(std::bind(&CountDownLatch::countDown, &latch));
    latch.wait();
    pool.stop();

    puts("=== ok. ===");

    return 0;
}