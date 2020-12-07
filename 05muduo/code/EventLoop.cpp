#include "EventLoop.h"

#include <assert.h>
#include <poll.h>

#include "Logging.h"

__thread EventLoop * t_loopInThisThread = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::tid())
{
    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another event loop " << t_loopInThisThread << " exists in this thread " << m_threadId;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!m_bLooping);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    assertInLoopThread();
    m_bLooping = true;

    ::poll(NULL, 0, 5 * 1000);

    m_bLooping = false;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "abortNotInLoopthread: " << m_threadId << " current thread is " << CurrentThread::tid();
}