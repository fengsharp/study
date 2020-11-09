#ifndef BOUNDED_BLOCKING_QUEUE_H_
#define BOUNDED_BLOCKING_QUEUE_H_

#include <assert.h>
#include <deque>

#include "Condition.h"
#include "Mutex.h"
#include "NoneCopyable.h"

template <typename T>
class BoundedBlockingQueue : private NoneCopyable
{
public:
    explicit BoundedBlockingQueue(unsigned maxSize)
        : m_maxSize(maxSize)
        , m_mutex()
        , m_notEmpty(m_mutex)
        , m_notFull(m_mutex)
    {
    }

    void put(const T & x)
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.size() == m_maxSize)
        {
            m_notFull.wait();
        }
        assert(m_queue.size() < m_maxSize);
        m_queue.push_back(x);
        m_notEmpty.notify();
    }

    T take()
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.empty())
        {
            m_notEmpty.wait();
        }
        assert(!m_queue.empty());
        T front(m_queue.front());
        m_queue.pop_front();
        m_notFull.notify();
        return front;
    }

    bool empty() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.empty();
    }

    bool full() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size() == m_maxSize;
    }

    size_t size() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size();
    }

    size_t capacity() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.capacity();
    }

private:
    const unsigned m_maxSize;
    mutable MutexLock m_mutex;
    Condition m_notEmpty GUARDED_BY(m_mutex);
    Condition m_notFull GUARDED_BY(m_mutex);
    std::deque<T> m_queue GUARDED_BY(m_mutex);
};

#endif