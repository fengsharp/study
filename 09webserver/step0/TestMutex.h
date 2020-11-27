#ifndef TEST_MUTEX_H_
#define TEST_MUTEX_H_

namespace threadtest
{
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "NoneCopyable.h"
#include "assert.h"

class MutexLock : private NoneCopyable
{
public:
    MutexLock()
        : m_holder(0)
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    ~MutexLock()
    {
        assert(m_holder == 0);
        pthread_mutex_destroy(&m_mutex);
    }

    bool isLockedByThisThread()
    {
        return m_holder == gettid();
    }

    void assertLocked()
    {
        assert(isLockedByThisThread());
    }

    void lock()
    {
        pthread_mutex_lock(&m_mutex);
        m_holder = gettid();
    }

    void unlock()
    {
        m_holder = 0;
        pthread_mutex_unlock(&m_mutex);
    }

    pthread_mutex_t * getPthreadMutex()
    {
        return &m_mutex;
    }

private:
    static pid_t gettid()
    {
        return static_cast<pid_t>(syscall(SYS_gettid));
    }

private:
    pthread_mutex_t m_mutex;
    pid_t m_holder;
};

class MutexLockGuard : private NoneCopyable
{
public:
    explicit MutexLockGuard(MutexLock & mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~MutexLockGuard()
    {
        m_mutex.unlock();
    }

private:
    MutexLock & m_mutex;
};

#define MutexLockGurad(x) error "Missing guard object name"

} // namespace threadtest

#endif