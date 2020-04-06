#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include <deque>
#include <assert.h>

#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"

template<typename T>
class BoundedBlockingQueue : noncopyable
{
public:
    explicit BoundedBlockingQueue(unsigned maxSize)
        : m_maxSize(maxSize), m_mutex(), m_notEmptyCond(m_mutex), m_notFullCond(m_mutex)
    {
    }

    void put(const T& x)
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.size() == m_maxSize)
        {
            m_notFullCond.wait();
        }
        assert(m_queue.size() < m_maxSize);
        m_queue.push_back(x);
        m_notEmptyCond.notify();
    }

    T take()
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.empty())
        {
            m_notEmptyCond.wait();
        }
        assert(!m_queue.empty());
        T front(m_queue.front());
        m_queue.pop_front();
        m_notFullCond.notify();
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
    const unsigned  m_maxSize;
    mutable MutexLock m_mutex;
    Condition  m_notEmptyCond;
    Condition  m_notFullCond;
    std::deque<T> m_queue;
};


#endif