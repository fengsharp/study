#ifndef CONDITION_H_
#define CONDITION_H_

#include <pthread.h>

#include "Mutex.h"
#include "NoneCopyable.h"

class Condition : private NoneCopyable 
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