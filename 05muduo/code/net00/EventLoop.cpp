#include "EventLoop.h"

#include <assert.h>
#include <pthread.h>

#include "Logging.h"

__thread EventLoop * t_loopInThisThread = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::tid())
{
    if (t_loopInThisThread == NULL)
    {
        t_loopInThisThread = this;
    }
    else
    {
        LOG_FATAL << "one thread one loop";
    }
}

EventLoop::~EventLoop()
{
    assert(m_bLooping == false);

    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(m_bLooping == false);

    m_bLooping = true;

    CurrentThread::sleepUsec(2 * 1000 * 1000);
    LOG_TRACE << "EventLoop " << this << " stop looping";

    m_bLooping = false;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "one thread one loop. old threadid " << m_threadId
              << ". now threadid " << CurrentThread::tid();
}