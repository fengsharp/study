#ifndef COUNT_DOWN_LATCH_H_
#define COUNT_DOWN_LATCH_H_

#include "Condition.h"
#include "Mutex.h"

class CountDownLatch : private NonCopyable
{
public:
    explicit CountDownLatch(int count)
        : m_mutex()
        , m_condition(m_mutex)
        , m_count(count)
    {
    }

    void wait()
    {
        MutexLockGuard lock(m_mutex);
        while (m_count > 0)
        {
            m_condition.wait();
        }
    }

    void countDown()
    {
        MutexLockGuard lock(m_mutex);
        --m_count;
        if (m_count == 0)
        {
            m_condition.notifyAll();
        }
    }

    int getCount() const
    {
        MutexLockGuard lock(m_mutex);
        return m_count;
    }

private:
    mutable MutexLock m_mutex;
    Condition m_condition GUARDED_BY(m_mutex);
    int m_count GUARDED_BY(m_mutex);
};

#endif