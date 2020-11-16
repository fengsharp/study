#include "EventLoop.h"

#include "Logging.h"

#include <poll.h>

__thread EventLoop * s_t_loopInThread = 0;

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return s_t_loopInThread;
}

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::gettid())
{
    LOG_TRACE << "eventloop create. address:" << this << ". thread id:" << m_threadId;
    if (s_t_loopInThread != nullptr)
    {
        LOG_FATAL << "s_t_loopInThread != nullptr. address: " << this << ". create thread id:" << m_threadId;
    }
    else
    {
        s_t_loopInThread = this;
    }
}

EventLoop::~EventLoop()
{
    s_t_loopInThread = nullptr;
}

void EventLoop::loop()
{
    assert(m_bLooping == false);

    assertInLoopThread();
    m_bLooping = true;
    LOG_TRACE << "EventLoop " << this << " start loop";

    ::poll(NULL, 0, 5 * 1000);

    LOG_TRACE << "EventLoop " << this << " end loop";
    m_bLooping = false;
}

void EventLoop::assertInLoopThread()
{
    if (isInLoopThread() == false)
    {
        abortNotInLoopThread();
    }
}

bool EventLoop::isInLoopThread()
{
    return m_threadId == CurrentThread::gettid();
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop " << this << " was created in thread id: "
              << m_threadId << ", current thread id: " << CurrentThread::gettid();
}