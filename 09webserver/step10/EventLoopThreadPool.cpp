#include "EventLoopThreadPool.h"

#include <stdio.h>

#include "EventLoop.h"
#include "EventLoopThread.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop * pBaseLoop, const string & nameArg)
    : m_pBaseLoop(pBaseLoop)
    , m_strName(nameArg)
    , m_bStarted(false)
    , m_numThreads(0)
    , m_next(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
    // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start(const ThreadInitCallback & cb)
{
    assert(!m_bStarted);
    m_pBaseLoop->assertInLoopThread();

    m_bStarted = true;

    for (int i = 0; i < m_numThreads; ++i)
    {
        char buf[m_strName.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", m_strName.c_str(), i);
        EventLoopThread * t = new EventLoopThread(cb, buf);
        m_vecThreads.push_back(std::unique_ptr<EventLoopThread>(t));
        m_vecLoops.push_back(t->startLoop());
    }

    if (m_numThreads == 0 && cb)
    {
        cb(m_pBaseLoop);
    }
}

EventLoop * EventLoopThreadPool::getNextLoop()
{
    m_pBaseLoop->assertInLoopThread();
    assert(m_bStarted);
    EventLoop * loop = m_pBaseLoop;

    if (!m_vecLoops.empty())
    {
        // round-robin
        loop = m_vecLoops[m_next];
        ++m_next;
        if (implicit_cast<size_t>(m_next) >= m_vecLoops.size())
        {
            m_next = 0;
        }
    }
    return loop;
}

EventLoop * EventLoopThreadPool::getLoopForHash(size_t hashCode)
{
    m_pBaseLoop->assertInLoopThread();
    EventLoop * loop = m_pBaseLoop;

    if (!m_vecLoops.empty())
    {
        loop = m_vecLoops[hashCode % m_vecLoops.size()];
    }
    return loop;
}

std::vector<EventLoop *> EventLoopThreadPool::getAllLoops()
{
    m_pBaseLoop->assertInLoopThread();
    assert(m_bStarted);
    if (m_vecLoops.empty())
    {
        return std::vector<EventLoop *>(1, m_pBaseLoop);
    }
    else
    {
        return m_vecLoops;
    }
}
