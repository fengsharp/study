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

    std::vector<Channel *> vecChannels;
    Timestamp receiveTime;
    while (m_bQuit == false)
    {
        vecChannels.clear();
        receiveTime = m_poller.poll(-1, &vecChannels);

        for (auto & item : vecChannels)
        {
            item->handle(receiveTime);
        }
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

void EventLoop::updateChannel(Channel * pChannel)
{
    assertLoopInThread();
    m_poller.updateChannel(pChannel);
}

void EventLoop::removeChannel(Channel * pChannel)
{
    assertLoopInThread();
    m_poller.removeChannel(pChannel);
}
