#include "EventLoop.h"

#include <assert.h>

#include "CurrentThread.h"
#include "Channel.h"
#include "Poller.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_threadId(CurrentThread::tid())
    , m_bQuit(false)
    , m_pPoller(new Poller(this))
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
    assertInLoopThread();
    m_bLooping = true;
    m_bQuit = false;

    Timestamp pollRetrunTime;
    while (!m_bQuit)
    {
        m_vecActiveChannels.clear();
        pollRetrunTime = m_pPoller->poll(10000, &m_vecActiveChannels);
        for (auto & item : m_vecActiveChannels)
        {
            item->handleEvent(pollRetrunTime);
        }
    }

    m_bLooping = false;
}

void EventLoop::quit()
{
    m_bQuit = true;
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    m_pPoller->updateChannel(pChannel);
}

bool EventLoop::isInLoopThread()
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertInLoopThread()
{
    if (isInLoopThread() == false)
    {
        abort();
    }
}

