#include "EventLoop.h"

#include <assert.h>

#include "Logging.h"
#include "Channel.h"
#include "Poller.h"
#include "TimerQueue.h"

__thread EventLoop * t_loopInThisThread = NULL;
const int kPollTimeMs = 10000; // 10 * 1000 ms

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_threadId(CurrentThread::tid())
    , m_pPoller(new Poller(this))
    , m_pTimerQueue(new TimerQueue(this))
{
    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another event loop " << t_loopInThisThread << " exists in this thread " << m_threadId;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!m_bLooping);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    assertInLoopThread();
    m_bLooping = true;
    m_bQuit = false;

    while (!m_bQuit)
    {
        m_activeChannels.clear();
        m_pollRetrunTime = m_pPoller->poll(kPollTimeMs, &m_activeChannels);
        
        for (auto & item : m_activeChannels)
        {
            item->handleEvent();
        }
    }

    m_bLooping = false;
}

void EventLoop::quit()
{
    m_bQuit = true;
    // wakeup
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    m_pPoller->updateChannel(pChannel);
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "abortNotInLoopthread: " << m_threadId << " current thread is " << CurrentThread::tid();
}

TimerId EventLoop::runAt(const Timestamp & time, const TimerCallback & cb)
{
    return m_pTimerQueue->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback & cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback & cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return m_pTimerQueue->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
    return m_pTimerQueue->cancel(timerId);
}
