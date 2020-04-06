#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include <assert.h>
#include <pthread.h>

#include "noncopyable.h"
#include "CurrentThread.h"

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

class Condition;

class MutexLock : noncopyable
{
public:
    MutexLock()
        : m_holder(0)
    {
        MCHECK(pthread_mutex_init(&m_mutex, NULL));
    }

    ~MutexLock()
    {
        assert(m_holder == 0);
        MCHECK(pthread_mutex_destroy(&m_mutex));
    }

    bool isLockedByThisThread() const {
        return m_holder == CurrentThread::tid();
    }

    void assertLocked() const {
        assert(isLockedByThisThread());
    }

    void lock() {
        MCHECK(pthread_mutex_lock(&m_mutex));
        assignHolder();
    }

    void unlock()
    {
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&m_mutex));
    }
    
private:
    pthread_mutex_t* getPthreadMutex() /* non-const */
    {
        return &m_mutex;
    }

    friend class Condition;
    class UnassignGuard : noncopyable
    {
    public:
        explicit UnassignGuard(MutexLock& owner)
        : m_owner(owner)
        {
            m_owner.unassignHolder();
        }

        ~UnassignGuard()
        {
            m_owner.assignHolder();
        }

    private:
        MutexLock& m_owner;
    };

    
    void assignHolder()
    {
        m_holder = CurrentThread::tid();
    }
    
    void unassignHolder()
    {
        m_holder = 0;
    }
private:
    pthread_mutex_t m_mutex;
    pid_t m_holder;
};

class MutexLockGuard : noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~MutexLockGuard()
    {
        m_mutex.unlock();
    }

private:
    MutexLock& m_mutex;
};

#define MutexLockGuard(x) error "Missing guard object name"

#endif