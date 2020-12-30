#ifndef AYSNC_LOGGING_H_
#define AYSNC_LOGGING_H_

#include <atomic>
#include <vector>

#include "BlockingQueue.h"
#include "BoundedBlockingQueue.h"
#include "CountDownLatch.h"
#include "LogStream.h"
#include "Mutex.h"
#include "Thread.h"

class AsyncLogging : private NonCopyable
{
public:
    AsyncLogging(const std::string & basename, off_t rollSize, int flushInterval = 3);
    ~AsyncLogging()
    {
        if (m_running)
        {
            stop();
        }
    }

    void append(const char * logline, int len);
    void start()
    {
        m_running = true;
        m_thread.start();
        m_latch.wait();
    }

    void stop() NO_THREAD_SAFETY_ANALYSIS
    {
        m_running = false;
        m_cond.notify();
        m_thread.join();
    }

private:
    void threadFunc();

private:
    const int m_flushInterval;
    std::atomic<bool> m_running;
    const std::string m_basename;
    const off_t m_rollSize;

    Thread m_thread;
    CountDownLatch m_latch;

    MutexLock m_mutex;
    Condition m_cond GUARDED_BY(m_mutex);

    typedef FixedBuffer<kLargeBuffer> Buffer;
    std::unique_ptr<Buffer> m_currentBuffer GUARDED_BY(m_mutex);
    std::unique_ptr<Buffer> m_nextBuffer GUARDED_BY(m_mutex);
    std::vector<std::unique_ptr<Buffer>> m_vecBuffers GUARDED_BY(m_mutex);
};

#endif