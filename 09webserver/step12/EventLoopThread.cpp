#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback & cb /* = ThreadInitCallback()*/, const string & name /* = string()*/)
    : m_pLoop(nullptr)
    , m_bExiting(false)
    , m_thread(std::bind(&EventLoopThread::threadFunc, this), name)
    , m_mutex()
    , m_cond(m_mutex)
    , m_initCallback(cb)
{
}

EventLoopThread::~EventLoopThread()
{
    m_bExiting = true;
    if (m_pLoop != nullptr)
    {
        m_pLoop->quit();
        m_thread.join();
    }
}

EventLoop * EventLoopThread::startLoop()
{
    assert(!m_thread.started());
    m_thread.start();

    EventLoop * pLoop = nullptr;
    {
        MutexLockGuard lock(m_mutex);
        while (m_pLoop == nullptr)
        {
            m_cond.wait();
        }
        pLoop = m_pLoop;
    }

    return pLoop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    if (m_initCallback)
    {
        m_initCallback(&loop);
    }

    {
        MutexLockGuard lock(m_mutex);
        m_pLoop = &loop;
        m_cond.notify();
    }

    loop.loop();
    MutexLockGuard lock(m_mutex);
    m_pLoop = nullptr;
}