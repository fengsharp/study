#include <stdio.h>

#include <memory>
#include <string>
#include <vector>

#include "CountDownLatch.h"
#include "CurrentThread.h"
#include "Thread.h"

constexpr int THR_NUM = 10;
CountDownLatch latch(THR_NUM);

void workFunc(int index)
{
    latch.wait();
    printf("index: %02d, tid: %d\n", index, CurrentThread::gettid());
}

int main()
{
    std::vector<std::shared_ptr<Thread>> vecThreads;

    for (int i = 0; i < THR_NUM; i++)
    {
        std::string thrName = "thread" + std::to_string(i);
        std::shared_ptr<Thread> pThread = std::make_shared<Thread>(std::bind(workFunc, i), thrName);
        vecThreads.push_back(pThread);
    }

    for (std::shared_ptr<Thread> & pThread : vecThreads)
    {
        pThread->start();
        latch.countDown();
    }
    
    for (std::shared_ptr<Thread> & pThread : vecThreads)
    {
        pThread->join();
    }

    return 0;
}