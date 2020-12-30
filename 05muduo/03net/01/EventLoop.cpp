#include "EventLoop.h"

#include <assert.h>

#include "CurrentThread.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::tid())
{
    if (gt_pLoop != NULL)
    {
        abort();
    }
    else
    {
        gt_pLoop = this;
    }
}

EventLoop::~EventLoop()
{
    assert(m_bLooping == false);
    gt_pLoop = NULL;
}

void EventLoop::loop()
{
    assert(m_bLooping == false);
    m_bLooping = true;

    CurrentThread::sleepUsec(1000 * 1000);

    m_bLooping = false;
}

bool EventLoop::isLoopInThread()
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertInLoopThread()
{
    if (isLoopInThread() == false)
    {
        abort();
    }
}