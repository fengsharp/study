#include <map>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "Thread.h"
#include "MutexLock.h"
#include "Timestamp.h"

MutexLock g_mutex;
std::map<int, int> g_delays;
void threadFunc()
{
    //printf("tid=%d\n", CurrentThread::tid());
}

void threadFunc2(Timestamp start)
{
    Timestamp now(Timestamp::now());
    int delay = static_cast<int>(timeDifference(now, start) * 1000000);
    MutexLockGuard lock(g_mutex);
    ++g_delays[delay];
}


int main(int argc, char* argv[])
{
    printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());
    Timestamp start(Timestamp::now());

    int kThreads = 100*1000;
    for (int i = 0; i < kThreads; ++i)
    {
        Thread t1(threadFunc);
        t1.start();
        t1.join();
    }

    double timeUsed = timeDifference(Timestamp::now(), start);
    printf("thread creation time %f us\n", timeUsed*1000000/kThreads);
    printf("number of created threads %d\n",Thread::numCreated());

    for (int i = 0; i < kThreads; ++i)
    {
        Timestamp now(Timestamp::now());
        Thread t2(std::bind(threadFunc2, now));
        t2.start();
        t2.join();
    }
    
    {
        MutexLockGuard lock(g_mutex);
        for (const auto& delay : g_delays)
        {
            printf("delay = %d, count = %d\n", delay.first, delay.second);
        }
    }

    return 0;
}