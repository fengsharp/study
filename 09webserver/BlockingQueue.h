#ifndef BLOCKING_QUEUE_H_
#define BLOCKING_QUEUE_H_

#include <assert.h>
#include <deque>

#include "Condition.h"
#include "Mutex.h"
#include "NoneCopyable.h"

template <typename T>
class BlockingQueue : private NoneCopyable
{
public:
    BlockingQueue()
        : m_mutex(), m_notEmpty(m_mutex), m_queue()
    {
    }
    
    void put(const T& x)
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(x);
        m_notEmpty.notify();
    }

    void put(T&& x)
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
        assert(!m_queue.empty());
        T front(std::move(m_queue.front()));
        m_queue.pop_front();
        return front;
    }

    size_t size() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size();
    }
private:
    mutable MutexLock m_mutex;
    Condition m_notEmpty GUARDED_BY(m_mutex);
    std::deque<T> m_queue GUARDED_BY(m_mutex);
};

#endif