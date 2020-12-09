#include "EventLoopThread.h"

#include "EventLoop.h"

EvnetLoopThread::EvnetLoopThread()
    : m_pLoop(NULL)
    , m_bExiting(false)
    , m_thread(std::bind(&EvnetLoopThread::threadFunc, this), "event_loop_thread")
    , m_mutex()
    , m_cond(m_mutex)
{
}

EvnetLoopThread::~EvnetLoopThread()
{
    m_bExiting = true;
    m_pLoop->quit();
    m_thread.join();
}

EventLoop * EvnetLoopThread::startLoop()
{
    m_thread.start();

    {
        MutexLockGuard lock(m_mutex);
        while (m_pLoop == NULL)
        {
            m_cond.wait();
        }
    }

    return m_pLoop;
}

void EvnetLoopThread::threadFunc()
{
    EventLoop loop;
    {
        MutexLockGuard lock(m_mutex);
        m_pLoop = &loop;
        m_cond.notify();
    }
    
    loop.loop();
}