#include "EventLoop.h"

#include <assert.h>
#include <vector>

#include "Channel.h"
#include "CurrentThread.h"

__thread EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_threadId(CurrentThread::tid())
    , m_poller(this)
    , m_timerQueue(this)
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

void EventLoop::runAt(const Timestamp & time, const TimerCallback & cb)
{
    m_timerQueue.addTimer(cb, time, 0.0);
}

void EventLoop::runAfter(double delay, const TimerCallback & cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    runAt(time, cb);
}

void EventLoop::runEvery(double interval, const TimerCallback & cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    m_timerQueue.addTimer(cb, time, interval);
}