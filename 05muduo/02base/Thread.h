#ifndef THREAD_H_
#define THREAD_H_

#include <functional>
#include <memory>
#include <pthread.h>

#include "Atomic.h"
#include "NonCopyable.h"
#include "Types.h"
#include "CountDownLatch.h"

typedef std::function<void()> ThreadFunc;
class Thread : private NonCopyable
{
public:
    Thread(const ThreadFunc & func, const std::string & name);
    ~Thread();

    void start();
    void join();

    bool started() const
    {
        return m_bStarted;
    }

    pid_t tid() const
    {
        return m_tid;
    }

    const std::string& name() const
    {
        return m_strName;
    }

    static int numCreated()
    {
        return m_numCreated.get();
    }

private:
    static void * thread_work(void * args);

private:
    ThreadFunc m_func;
    std::string m_strName;

    bool m_bStarted;
    bool m_bJoined;
    pthread_t m_pthreadId;
    pid_t m_tid;
    CountDownLatch m_latch;

    static AtomicInt32 m_numCreated;
};

#endif