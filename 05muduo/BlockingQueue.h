#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H


#include <deque>
#include <assert.h>

#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"

template<typename T>
class BlockingQueue : noncopyable
{
public:
    BlockingQueue()
        : m_mutex(), m_notEmptyCond(m_mutex), m_queue()
    {
    }

    void put(const T& x)
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(x);
        m_notEmptyCond.notify();
    }

    void put(T&& x) // 临时变量，或者std::move
    {
        MutexLockGuard lock(m_mutex);
        m_queue.push_back(std::move(x));
        m_notEmptyCond.notify();
    }

    T take()
    {
        MutexLockGuard lock(m_mutex);
        // always use a while-loop, due to spurious wakeup
        while (m_queue.empty()) {
            m_notEmptyCond.wait();
        }
        assert(!m_queue.empty());
        T front(std::move(m_queue.front()));
        m_queue.pop_front();
        return std::move(front);
    }

    size_t size() const
    {
        MutexLockGuard lock(m_mutex);
        return m_queue.size();
    }
private:
    mutable MutexLock m_mutex;
    Condition m_notEmptyCond;
    std::deque<T> m_queue;
};

#endif