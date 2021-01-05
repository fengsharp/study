#include "EventLoop.h"

#include <assert.h>

#include "CurrentThread.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
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
    assertLoopInThread();
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
    wakeup();
}

bool EventLoop::isLoopInThread() const
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertLoopInThread()
{
    if (!isLoopInThread())
    {
        abort();
    }
}

void EventLoop::wakeup()
{

}















