#ifndef BLOCKING_QUEUE_H_
#define BLOCKING_QUEUE_H_

#include <list>

#include "Condition.h"
#include "Mutex.h"

template <typename T>
class BlockingQueue : private NonCopyable
{
public:
    BlockingQueue()
        : m_mutex()
        , m_notEmptyCondition(m_mutex)
        , m_queue()
    {
    }

    void put(const T & x)
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(x);
        m_notEmptyCondition.notify();
    }

    void put(T && x)
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(std::move(x));
        m_notEmptyCondition.notify();
    }

    T take()
    {
        MutexLockGuard lock(m_mutex);
        while (m_queue.empty())
        {
            m_notEmptyCondition.wait();
        }

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
    MutexLock m_mutex;
    Condition m_notEmptyCondition GUARDED_BY(m_mutex);
    std::list<T> m_queue GUARDED_BY(m_mutex);
};

#endif