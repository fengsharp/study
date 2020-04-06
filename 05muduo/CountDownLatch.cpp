#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
  : m_mutex(),
    m_conditon(m_mutex),
    m_count(count)
{
}

void CountDownLatch::wait()
{
    MutexLockGuard lock(m_mutex);
    while (m_count > 0)
    {
        m_conditon.wait();
    }
}

void CountDownLatch::countDown()
{
    MutexLockGuard lock(m_mutex);
    --m_count;
    if (m_count == 0)
    {
        m_conditon.notifyAll();
    }
}

int CountDownLatch::getCount() const
{
    MutexLockGuard lock(m_mutex);
    return m_count;
}

