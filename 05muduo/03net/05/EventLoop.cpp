#include "EventLoop.h"

#include <assert.h>

#include <vector>

#include "Channel.h"
#include "CurrentThread.h"

EventLoop * gt_pLoop = NULL;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_threadId(CurrentThread::tid())
    , m_poller(this)
    , m_timerQueue(this)
    , m_pendingQueue(this)
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
    m_bQuit = false;

    std::vector<Channel *> vecActiveChannels;
    while (m_bQuit == false)
    {
        vecActiveChannels.clear();
        Timestamp receiveTime = m_poller.poll(-1, &vecActiveChannels);

        for (auto & item : vecActiveChannels)
        {
            item->handle(receiveTime);
        }

        m_pendingQueue.callPendingFunctiors();
    }

    m_bLooping = false;
}

void EventLoop::stop()
{
    m_bQuit = true;
    if (!isLoopInThread())
    {
        m_pendingQueue.wakeup();
    }
}

bool EventLoop::isLoopInThread()
{
    return m_threadId == CurrentThread::tid();
}

void EventLoop::assertLoopInThread()
{
    if (isLoopInThread() == false)
    {
        abort();
    }
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assertLoopInThread();
    m_poller.updateChannel(pChannel);
}

TimerId EventLoop::runAt(const Timestamp & time, const TimerCallback & cb)
{
    return m_timerQueue.addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback & cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback & cb)
{
    return m_timerQueue.addTimer(cb, addTime(Timestamp::now(), interval), interval);
}

void EventLoop::runInLoop(const PendingCallback & cb)
{
    if (isLoopInThread())
    {
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const PendingCallback & cb)
{
    m_pendingQueue.addCallback(cb);
}
