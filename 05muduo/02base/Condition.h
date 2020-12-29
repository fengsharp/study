#ifndef CONDITION_H_
#define CONDITION_H_

#include <pthread.h>

#include "Mutex.h"
#include "NonCopyable.h"

class Condition : private NonCopyable
{
public:
    explicit Condition(MutexLock & mutex)
        : m_mutex(mutex)
    {
        pthread_cond_init(&m_cond, NULL);
    }

    ~Condition()
    {
        pthread_cond_destroy(&m_cond);
    }

    void wait()
    {
        MutexLock::UnassignGuard ug(m_mutex);
        pthread_cond_wait(&m_cond, m_mutex.getPthreadMutex());
    }

    void notify()
    {
        pthread_cond_signal(&m_cond);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&m_cond);
    }

    bool waitForSeconds(double seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);

        const int64_t kNanoSecondsPerSecond = 1000000000;
        int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

        abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
        abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

        MutexLock::UnassignGuard ug(m_mutex);
        return ETIMEDOUT == pthread_cond_timedwait(&m_cond, m_mutex.getPthreadMutex(), &abstime);
    }

private:
    MutexLock & m_mutex;
    pthread_cond_t m_cond;
};

#endif