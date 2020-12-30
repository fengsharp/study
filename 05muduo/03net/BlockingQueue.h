#ifndef BLOCKING_QUEUE_H_
#define BLOCKING_QUEUE_H_

#include <assert.h>

#include <list>

#include "Condition.h"
#include "Mutex.h"

template <typename T>
class BlockingQueue
{
public:
    BlockingQueue()
        : m_mutex()
        , m_notEmpty(m_mutex)
        , m_queue()
    {
    }

    void put(const T & x)
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(x);
        m_notEmpty.notify();
    }

    void put(const T && x)
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(std::move(x));
        m_notEmpty.notify();
    }

    T take()
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.empty())
        {
            m_notEmpty.wait();
        }

        T ret = std::move(m_queue.front());
        m_queue.pop_front();
        return ret;
    }

    size_t size() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size();
    }

private:
    mutable MutexLock m_mutex;
    Condition m_notEmpty GUARDED_BY(m_mutex);
    std::list<T> m_queue GUARDED_BY(m_mutex);
};

#endif