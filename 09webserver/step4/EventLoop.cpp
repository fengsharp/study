#include "EventLoop.h"

#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <algorithm>

#include "Channel.h"
#include "Logging.h"
#include "Poller.h"
#include "TimerQueue.h"

__thread EventLoop * s_t_loopInThread = 0;
const int kPollTimeMs = 10000;

int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_SYSERR << "failed in eventfd";
        abort();
    }
    return evtfd;
}

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return s_t_loopInThread;
}

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_bEventHanding(false)
    , m_bCallingPendingFunctors(false)
    , m_threadId(CurrentThread::gettid())
    , m_pPoller(new Poller(this))
    , m_pTimerQueue(new TimerQueue(this))
    , m_wakeupfd(createEventfd())
    , m_pWakeupChannel(new Channel(this, m_wakeupfd))
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

    m_pWakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead, this, _1));
    m_pWakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
    m_pWakeupChannel->disableAll();
    m_pWakeupChannel->remove();
    ::close(m_wakeupfd);

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

        doPendingFunctors();
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
        wakeup();
    }
}

void EventLoop::runInLoop(Functors cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functors cb)
{
    {
        MutexLockGuard lock(m_mutex);
        m_vecPendingFunctors.push_back(cb);
    }

    if (!isInLoopThread() || m_bCallingPendingFunctors)
    {
        wakeup();
    }
}

size_t EventLoop::queueSize() const
{
    MutexLockGuard lock(m_mutex);
    return m_vecPendingFunctors.size();
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

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(m_wakeupfd, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR << "EventLoop::wakeup() writes" << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead(Timestamp receiveTime)
{
    uint64_t one = 1;
    ssize_t n = ::read(m_wakeupfd, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR << "EventLoop::handleRead() read" << n << " bytes instead of 8";
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functors> vecFunctors;
    m_bCallingPendingFunctors = true;

    {
        MutexLockGuard lock(m_mutex);
        vecFunctors.swap(m_vecPendingFunctors);
    }

    for (const Functors& item : vecFunctors)
    {
        item();
    }
    
    m_bCallingPendingFunctors = false;
}