#include "EventLoop.h"

#include <assert.h>
#include <pthread.h>

#include "Channel.h"
#include "Logging.h"
#include "Poller.h"
#include "TimerQueue.h"
#include "WeakupQueue.h"

__thread EventLoop * t_loopInThisThread = NULL;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_bEventHanding(false)
    , m_threadId(CurrentThread::tid())
    , m_pPoller(std::make_unique<Poller>(this))
    , m_pTimerQueue(std::make_unique<TimerQueue>(this))
    , m_pCurrentActiveChannel(NULL)
    , m_pWeakupQueue(std::make_unique<WeakupQueue>(this))
{
    assert(t_loopInThisThread == NULL);
    t_loopInThisThread = this;
}

EventLoop::~EventLoop()
{
    assert(m_bLooping == false);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(m_bLooping == false);
    m_bLooping = true;
    m_bQuit = false;

    while (m_bQuit == false)
    {
        m_vecActiveChannels.clear();
        m_pollReturnTime = m_pPoller->poll(kPollTimeMs, &m_vecActiveChannels);

        m_bEventHanding = true;
        for (auto & item : m_vecActiveChannels)
        {
            m_pCurrentActiveChannel = item;
            item->handleEvent(m_pollReturnTime);
        }
        m_pCurrentActiveChannel = NULL;
        m_bEventHanding = false;

        m_pWeakupQueue->doPendingFunctors();
    }

    m_bLooping = false;
}

void EventLoop::quit()
{
    m_bQuit = true;
    if (isInLoopThread() == false)
    {
        m_pWeakupQueue->wakeup();
    }
}

bool EventLoop::hasChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertLoopInThisThread();
    return m_pPoller->hasChannel(pChannel);
}

void EventLoop::removeChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertLoopInThisThread();
    m_pPoller->removeChannel(pChannel);
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertLoopInThisThread();
    if (m_bEventHanding)
    {
        assert(m_pCurrentActiveChannel == pChannel || std::find(m_vecActiveChannels.begin(), m_vecActiveChannels.end(), pChannel) == m_vecActiveChannels.end());
    }
    m_pPoller->updateChannel(pChannel);
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

TimerId EventLoop::runEvery(double inteterval, const TimerCallback & cb)
{
    Timestamp time(addTime(Timestamp::now(), inteterval));
    return m_pTimerQueue->addTimer(cb, time, inteterval);
}

void EventLoop::runInLoop(const Functor & cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor & cb)
{
    m_pWeakupQueue->push(cb);
}