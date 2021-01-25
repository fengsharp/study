#include "EventLoop.h"

#include <assert.h>

#include "CurrentThread.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::tid())
{
    assert(gt_pLoop == NULL);
    gt_pLoop = this;
}

EventLoop::~EventLoop()
{
    assert(m_bLooping == false);
    gt_pLoop = NULL;
}

void EventLoop::loop()
{
    assertLoopInThread();
    
    assert(m_bLooping == false);
    m_bLooping = true;

    CurrentThread::sleepUsec(1000 * 1000);
    puts("event loop.");

    m_bLooping = false;
}

bool EventLoop::isLoopInThread() const
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertLoopInThread() const
{
    if (!isLoopInThread())
    {
        abort();
    }
}
