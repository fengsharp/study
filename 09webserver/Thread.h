#ifndef THREAD_H_
#define THREAD_H_

#include <functional>
#include <memory>
#include <pthread.h>

#include "Atomic.h"
#include "Types.h"
#include "NoneCopyable.h"
#include "CountDownLatch.h"

class Thread : private NoneCopyable
{
public:
    explicit Thread(std::function<void()> funCallback, const string& name=string());
    ~Thread();

    void start();
    int join(); // return pthread_join
    
    bool started() const
    {
        return m_bStarted;
    }

    pid_t tid() const
    {
        return m_tid;
    }

    const string& name()
    {
        return m_strName;
    }

    static int numCreated()
    {
        return m_numCreated.get();
    }
private:
    void setDefaultName();

private:
    std::function<void ()> m_fnThreadCallback;
    bool m_bStarted;
    bool m_bJoined;
    pthread_t m_pthreadId;
    pid_t m_tid;
    string m_strName;
    CountDownLatch m_latch;

    static AtomicInt32 m_numCreated;
};

#endif