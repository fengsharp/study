#include "EventLoop.h"

#include "Logging.h"

#include <poll.h>
#include <stdio.h>

__thread EventLoop * t_loopInThisThread = nullptr;

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::gettid())
{
    // printf("log level: %d\n", Logger::logLevel());

    LOG_TRACE << "create eventloop, thread id: " << m_threadId;
    if (t_loopInThisThread)
    {
        LOG_FATAL << "one eventloop per thread. fatal thread id: " << m_threadId;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    t_loopInThisThread = nullptr;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "eventloop created thread id: " << m_threadId
              << ". But now current thread id: " << CurrentThread::gettid()
              << ".";
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    assertInLoopThread();
    m_bLooping = true;
    LOG_TRACE << "eventloop: " << this << " start loop";

    ::poll(NULL, 0, 5);

    LOG_TRACE << "eventloop: " << this << " end loop";
    m_bLooping = false;
}