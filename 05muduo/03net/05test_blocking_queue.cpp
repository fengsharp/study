#include <math.h>
#include <stdio.h>

#include <memory>

#include "BlockingQueue.h"
#include "CountDownLatch.h"
#include "NonCopyable.h"
#include "Thread.h"

const int NUM_STARTED = 30000000;
const int NUM_ENDED = 30002000;

bool isPrime(int num)
{
    if (num == 1)
        return false;
    if (num == 2)
        return true;
    for (int i = 2; i < sqrt(num) + 1; i++)
    {
        if ((num % i) == 0)
            return false;
    }
    return true;
}

class Work : private NonCopyable
{
public:
    explicit Work(int threadNum)
        : m_latch(threadNum)
    {
        for (int i = 0; i < threadNum; i++)
        {
            char name[32] = { 0 };
            snprintf(name, sizeof(name) - 1, "thread-%d", i);
            std::unique_ptr<Thread> pThread = std::make_unique<Thread>(std::bind(&Work::threadWork, this), name);
            m_vecThreads.emplace_back(std::move(pThread));
        }

        for (auto & item : m_vecThreads)
        {
            item->start();
        }        
    }

    void run(int startNum, int endNum)
    {
        m_latch.wait();

        for (int i = startNum; i <= endNum; i++)
        {
            m_blockQueue.put(i);
        }
    }

    void joinAll()
    {
        for (size_t i = 0; i < m_vecThreads.size(); i++)
        {
            m_blockQueue.put(0);
        }

        for (auto & item : m_vecThreads)
        {
            item->join();
        }
    }

private:
    void threadWork()
    {
        m_latch.countDown();

        while (true)
        {
            int num = m_blockQueue.take();
            if (num == 0)
            {
                break;
            }
            else
            {
                if (isPrime(num))
                {
                    printf("thread %s select num: %d\n", CurrentThread::name(), num);
                }
            }
        }
    }

private:
    BlockingQueue<int> m_blockQueue;
    CountDownLatch m_latch;
    std::vector<std::unique_ptr<Thread>> m_vecThreads;
};

int main()
{
    Work work(10);
    work.run(NUM_STARTED, NUM_ENDED);
    work.joinAll();

    if (CurrentThread::isMainThread())
    {
        printf("%s\n", CurrentThread::name());
        puts("=== ok. ===");
    }

    return 0;
}