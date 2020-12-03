#include <stdio.h>

#include "Thread.h"
#include "Mutex.h"

int g_num = 6000000;
const int cnt = g_num / 2;
MutexLock mutex;

void work()
{
    for (int i = 0; i < cnt; ++i)
    {
        MutexLockGuard lock(mutex);
        --g_num;
    }
}

int main()
{
    Thread th1(std::bind(work), "thread1");
    Thread th2(std::bind(work), "thread2");
    th1.start();
    th2.start();

    th1.join();
    th2.join();

    printf("result: %d\n", g_num);

    return 0;
}