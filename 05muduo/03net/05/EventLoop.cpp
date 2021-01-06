#include "EventLoop.h"

#include <assert.h>

#include <vector>

#include "CurrentThread.h"

#include "Channel.h"
#include "Poller.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_threadId(CurrentThread::tid())
    , m_poller(this)
    , m_timer(this)
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

    std::vector<Channel *> vecActivityChannels;
    Timestamp receivedTime;
    while (m_bQuit == false)
    {
        vecActivityChannels.clear();
        receivedTime = m_poller.poll(-1, &vecActivityChannels);
        for (auto & item : vecActivityChannels)
        {
            item->handleEvent(receivedTime);
        }
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

void EventLoop::updateChannel(Channel * pChannel)
{
    assertLoopInthread();
    assert(pChannel->ownerLoop() == this);
    m_poller.updateChannel(pChannel);
}

void EventLoop::runAt(Timestamp when, const TimerCallback & cb)
{
    m_timer.addTimer(cb, when, 0.0);
}

void EventLoop::runAfter(double delay, const TimerCallback & cb)
{
    Timestamp when(addTime(Timestamp::now(), delay));
    runAt(when, cb);
}

void EventLoop::runEvery(double interval, const TimerCallback & cb)
{
    m_timer.addTimer(cb, addTime(Timestamp::now(), interval), interval);
}
