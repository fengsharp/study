#include "EventLoop.h"

#include <assert.h>
#include <vector>

#include "CurrentThread.h"
#include "Channel.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuite(false)
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

    std::vector<Channel *> vecChannels;

    while (m_bQuite == false)
    {
        vecChannels.clear();
        Timestamp receivedTime = m_poller.poll(-1, &vecChannels);

        for (auto & item : vecChannels)
        {
            item->handle(receivedTime);
        }
    }
    
    m_bLooping = false;
}

void EventLoop::stop()
{
    m_bQuite = true;
    
    if (isLoopInThread() == false)
    {
        // wake up
    }
}

bool EventLoop::isLoopInThread() const
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertLoopInThread() const
{
    if (isLoopInThread() == false)
    {
        abort();
    }
}

void EventLoop::update(Channel * pChannel)
{
    assertLoopInThread();
    m_poller.updateChannel(pChannel);
}
