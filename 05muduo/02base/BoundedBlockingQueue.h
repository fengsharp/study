#ifndef BOUNDED_BLOCKING_QUEUE_H_
#define BOUNDED_BLOCKING_QUEUE_H_

#include <assert.h>

#include <list>

#include "Condition.h"
#include "Mutex.h"

template <typename T, int MAXQUEUE>
class BoundedBlockingQueue
{
public:
    BoundedBlockingQueue()
        : m_mutex()
        , m_notEmpty(m_mutex)
        , m_notFull(m_mutex)
        , m_queue()
    {
    }

    void put(const T & x)
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.size() > MAXQUEUE)
        {
            m_notFull.wait();
        }
        m_queue.push_back(x);
        m_notEmpty.notify();
    }

    void put(const T && x)
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.size() >= MAXQUEUE)
        {
            m_notFull.wait();
        }
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
        m_notFull.notify();

        return ret;
    }

    bool empty() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.empty();
    }

    bool full() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size() == MAXQUEUE;
    }

    size_t size() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size();
    }
private:
    mutable MutexLock m_mutex;
    Condition m_notEmpty GUARDED_BY(m_mutex);
    Condition m_notFull GUARDED_BY(m_mutex);
    std::list<T> m_queue GUARDED_BY(m_mutex);
};

#endif