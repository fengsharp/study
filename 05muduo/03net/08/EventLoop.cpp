#include "EventLoop.h"

#include <assert.h>
#include <vector>

#include "CurrentThread.h"
#include "Channel.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_threadId(CurrentThread::tid())
    , m_poller(this)
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

    std::vector<Channel *> pActiveChannels;
    Timestamp receivedTime;
    while (m_bQuit == false)
    {
        receivedTime = m_poller.poll(-1, &pActiveChannels);

        for (auto & item : pActiveChannels)
        {
            item->handleEvent();
        }
    }

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

void EventLoop::stop()
{
    m_bQuit = true;
    // wakeup
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assertLoopInThread();
    m_poller.updateChannel(pChannel);
}