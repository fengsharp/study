#include "EventLoop.h"

#include <assert.h>

#include "CurrentThread.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
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
    assertLoopInthread();
    assert(m_bLooping == false);
    m_bLooping = true;

    while (m_bQuit == false)
    {
        CurrentThread::sleepUsec(1000);
        puts("--- loop ---");
    }

    m_bLooping = false;
}

void EventLoop::stop()
{
    m_bQuit = true;
    if (isLoopInThread() == false)
    {
        // wakeup
    }
}

bool EventLoop::isLoopInThread() const
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertLoopInthread()
{
    if (isLoopInThread() == false)
    {
        abort();
    }
}
