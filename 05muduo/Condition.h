#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include <pthread.h>

#include "noncopyable.h"
#include "MutexLock.h"


class Condition : noncopyable
{
public:
    explicit Condition(MutexLock& mutex)
        : m_mutex(mutex)
    {
        MCHECK(pthread_cond_init(&m_cond, NULL));
    }
    ~Condition()
    {
        MCHECK(pthread_cond_destroy(&m_cond));
    }

    void wait()
    {
        MutexLock::UnassignGuard ug(m_mutex);
        MCHECK(pthread_cond_wait(&m_cond, m_mutex.getPthreadMutex()));
    }

    // returns true if time out, false otherwise.
    bool waitForSeconds(double seconds);

    void notify()
    {
        MCHECK(pthread_cond_signal(&m_cond));
    }

    void notifyAll()
    {
        MCHECK(pthread_cond_broadcast(&m_cond));
    }

private:
    MutexLock& m_mutex;
    pthread_cond_t m_cond;
};

#endif