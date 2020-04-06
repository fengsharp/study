#include <stdio.h>
#include <iostream>
#include <unistd.h>  // usleep
#include <string>

#include "CountDownLatch.h"
#include "Thread.h"
#include "ThreadPool.h"
#include "CurrentThread.h"

void print()
{
    printf("tid=%d\n", CurrentThread::tid());
}

void printString(const std::string& str)
{
    std::cout << str << std::endl;
    usleep(100*1000);
}

void test(int maxSize)
{
    std::cout << "Test ThreadPool with max queue size = " << maxSize << std::endl;
    ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    puts("Adding");
    pool.run(print);
    pool.run(print);
    for (int i = 0; i < 100; ++i)
    {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(std::bind(printString, std::string(buf)));
    }
    puts("Done");

    CountDownLatch latch(1);
    pool.run(std::bind(&CountDownLatch::countDown, &latch));
    latch.wait();
    pool.stop();
}

int main()
{
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
}
