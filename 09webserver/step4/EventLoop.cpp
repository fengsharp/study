#include "EventLoop.h"

#include "Channel.h"
#include "Logging.h"
#include "Poller.h"
#include "TimerQueue.h"

__thread EventLoop * s_t_loopInThread = 0;
const int kPollTimeMs = 10000;

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return s_t_loopInThread;
}

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_bEventHanding(false)
    , m_threadId(CurrentThread::gettid())
    , m_pPoller(new Poller(this))
    , m_pTimerQueue(new TimerQueue(this))
    , m_pCurrentActiveChannel(nullptr)
{
    LOG_TRACE << "eventloop create. address:" << this << ". thread id:" << m_threadId;
    if (s_t_loopInThread != nullptr)
    {
        LOG_FATAL << "s_t_loopInThread != nullptr. address: " << this << ". create thread id:" << m_threadId;
    }
    else
    {
        s_t_loopInThread = this;
    }
}

EventLoop::~EventLoop()
{
    s_t_loopInThread = nullptr;
}

void EventLoop::loop()
{
    assert(m_bLooping == false);

    assertInLoopThread();
    m_bLooping = true;
    m_bQuit = false;
    LOG_TRACE << "EventLoop " << this << " start loop";

    while (m_bQuit == false)
    {
        m_vecActiveChannels.clear();
        m_pollRetrunTime = m_pPoller->poll(kPollTimeMs, &m_vecActiveChannels);
        if (Logger::logLevel() <= Logger::TRACE)
        {
            printActiveChannels();
        }

        m_bEventHanding = true;
        for (Channel * pItem : m_vecActiveChannels)
        {
            m_pCurrentActiveChannel = pItem;
            m_pCurrentActiveChannel->handleEvent(m_pollRetrunTime);
        }
        m_pCurrentActiveChannel = nullptr;
        m_bEventHanding = false;
    }

    LOG_TRACE << "EventLoop " << this << " end loop";
    m_bLooping = false;
}

void EventLoop::assertInLoopThread()
{
    if (isInLoopThread() == false)
    {
        abortNotInLoopThread();
    }
}

bool EventLoop::isInLoopThread()
{
    return m_threadId == CurrentThread::gettid();
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop " << this << " was created in thread id: "
              << m_threadId << ", current thread id: " << CurrentThread::gettid();
}

void EventLoop::printActiveChannels() const
{
    for (Channel * pItem : m_vecActiveChannels)
    {
        LOG_TRACE << "{" << pItem->reventsToString() << "} ";
    }
}

void EventLoop::quit()
{
    m_bQuit = true;
    if (!isInLoopThread())
    {
        // wake up
    }
}

bool EventLoop::hasChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    return m_pPoller->hasChannel(pChannel);
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    m_pPoller->updateChannel(pChannel);
}

void EventLoop::removeChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    if (m_bEventHanding)
    {
        assert(m_pCurrentActiveChannel == pChannel || std::find(m_vecActiveChannels.begin(), m_vecActiveChannels.end(), pChannel) == m_vecActiveChannels.end());
    }
    m_pPoller->removeChannel(pChannel);
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