#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <deque>
#include <vector>

#include "Condition.h"
#include "Mutex.h"
#include "NoneCopyable.h"
#include "Thread.h"
#include "Types.h"
#include "Thread.h"

class ThreadPool : private NoneCopyable
{
public:
    explicit ThreadPool(const string & nameArgs = string("ThreadPool"));
    ~ThreadPool();

public:
    void setMaxQueueSize(int maxSize)
    {
        m_maxQueueSize = maxSize;
    }

    void setThreadInitCallback(const std::function<void()> & cb)
    {
        m_threadInitCallback = cb;
    }

    void start(int numThreads);
    void stop();

    const string & name() const
    {
        return m_strName;
    }

    size_t queueSize() const;
    void run(std::function<void()> task);

private:
    bool isFull() const REQUIRES(m_mutex);
    void runInThread();
    std::function<void()> take();

private:
    mutable MutexLock m_mutex;
    Condition m_notEmpty GUARDED_BY(m_mutex);
    Condition m_notFull GUARDED_BY(m_mutex);
    string m_strName;
    std::function<void()> m_threadInitCallback;
    std::vector<std::unique_ptr<Thread>> m_vecThreads;
    std::deque<std::function<void()>> m_queue GUARDED_BY(m_mutex);
    size_t m_maxQueueSize;
    bool m_isRunning;
};

#endif