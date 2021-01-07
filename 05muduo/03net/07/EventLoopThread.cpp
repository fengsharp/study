#include "EventLoopThread.h"

#include "EventLoop.h"

EventLoopThread::EventLoopThread()
    : m_pLoop(NULL)
    , m_mutex()
    , m_cond(m_mutex)
    , m_thread(std::bind(&EventLoopThread::threadCallback, this), "loop-thread")
{
}

EventLoopThread::~EventLoopThread()
{
    {
        MutexLockGuard lock(m_mutex);
        m_pLoop->stop();
        m_pLoop = NULL;
    }

    m_thread.join();
}

EventLoop * EventLoopThread::startLoop()
{
    m_thread.start();
    EventLoop * pLoop = NULL;
    {
        MutexLockGuard lock(m_mutex);
        while (m_pLoop == NULL)
        {
            m_cond.wait();
        }
        pLoop = m_pLoop;
    }
    return pLoop;
}

void EventLoopThread::threadCallback()
{
    EventLoop loop;
    {
        MutexLockGuard lock(m_mutex);
        m_pLoop = &loop;
        m_cond.notify();
    }

    loop.loop();
}
