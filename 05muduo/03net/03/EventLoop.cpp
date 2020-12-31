#include "EventLoop.h"

#include <assert.h>

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
    m_bLooping = true;

    std::vector<Channel *> vecReceiveChannel;
    Timestamp receiveTime;
    while (!m_bQuit)
    {
        vecReceiveChannel.clear();
        receiveTime = m_poller.poll(10000, &vecReceiveChannel);

        for (auto & item : vecReceiveChannel)
        {
            item->handle(receiveTime);
        }
    }

    m_bLooping = false;
}

void EventLoop::stop()
{
    m_bQuit = true;
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assertInLoopThread();
    m_poller.updateChannel(pChannel);
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
