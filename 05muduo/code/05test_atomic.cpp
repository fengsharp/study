#include <stdio.h>

#include "Thread.h"
#include "Atomic.h"

const int NUM = 6000000;
const int cnt = NUM / 2;
AtomicInt32 g_num;


void work()
{
    for (int i = 0; i < cnt; ++i)
    {
        g_num.decrement();
    }
}

int main()
{
    g_num.getAndSet(NUM);

    Thread th1(std::bind(work), "thread1");
    Thread th2(std::bind(work), "thread2");
    th1.start();
    th2.start();

    th1.join();
    th2.join();

    printf("result: %d\n", g_num.get());

    return 0;
}