#include "EventLoopThread.h"

#include <functional>

#include "EventLoop.h"

EventLoopThread::EventLoopThread()
    : m_pLoop(NULL)
    , m_thread(std::bind(&EventLoopThread::threadFunc, this), "loop-thread")
    , m_mutex()
    , m_cond(m_mutex)
{
}

EventLoopThread::~EventLoopThread()
{
    m_pLoop->stop();
    m_thread.join();
}

EventLoop * EventLoopThread::startLoop()
{
    assert(m_thread.started() == false);

    m_thread.start();
    EventLoop * loop = NULL;
    {
        MutexLockGuard lock(m_mutex);
        while (m_pLoop == NULL)
        {
            m_cond.wait();
        }
        loop = m_pLoop;
    }

    return loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    {
        MutexLockGuard lock(m_mutex);
        m_pLoop = &loop;
        m_cond.notify();
    }
    loop.loop();
}