#include <map>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "BlockingQueue.h"
#include "CountDownLatch.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Types.h"

class Bench
{
public:
    Bench(int numThreads)
        : latch_(numThreads)
    {
        for (int i = 0; i < numThreads; ++i)
        {
            char name[32];
            snprintf(name, sizeof name, "work thread %d", i);
            threads_.emplace_back(new Thread(
                std::bind(&Bench::threadFunc, this), string(name)));
        }
        for (auto & thr : threads_)
        {
            thr->start();
        }
    }

    void run(int times)
    {
        printf("waiting for count down latch\n");
        latch_.wait();
        printf("all threads started\n");
        for (int i = 0; i < times; ++i)
        {
            Timestamp now(Timestamp::now());
            queue_.put(now);
            usleep(1000);
        }
    }

    void joinAll()
    {
        for (size_t i = 0; i < threads_.size(); ++i)
        {
            queue_.put(Timestamp::invalid());
        }

        for (auto & thr : threads_)
        {
            thr->join();
        }
    }

private:
    void threadFunc()
    {
        printf("tid=%d, %s started\n",
               CurrentThread::tid(),
               CurrentThread::name());

        std::map<int, int> delays;
        latch_.countDown();
        bool running = true;
        while (running)
        {
            Timestamp t(queue_.take());
            Timestamp now(Timestamp::now());
            if (t.isValid())
            {
                int delay = static_cast<int>(timeDifference(now, t) * 1000000);
                // printf("tid=%d, latency = %d us\n",
                //        CurrentThread::tid(), delay);
                ++delays[delay];
            }
            running = t.isValid();
        }

        printf("tid=%d, %s stopped\n",
               CurrentThread::tid(),
               CurrentThread::name());
        for (const auto & delay : delays)
        {
            printf("tid = %d, delay = %d, count = %d\n",
                   CurrentThread::tid(),
                   delay.first, delay.second);
        }
    }

    BlockingQueue<Timestamp> queue_;
    CountDownLatch latch_;
    std::vector<std::unique_ptr<Thread>> threads_;
};

int main(int argc, char * argv[])
{
    int threads = argc > 1 ? atoi(argv[1]) : 1;

    Bench t(threads);
    t.run(100);
    t.joinAll();
}
