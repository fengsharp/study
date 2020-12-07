#include "EventLoop.h"

#include <assert.h>
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "Channel.h"
#include "Logging.h"
#include "Poller.h"
#include "TimerQueue.h"

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

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE, SIG_IGN);
        // LOG_TRACE << "Ignore SIGPIPE";
    }
};
#pragma GCC diagnostic error "-Wold-style-cast"
IgnoreSigPipe initObj;

__thread EventLoop * t_loopInThisThread = NULL;
const int kPollTimeMs = 10000; // 10 * 1000 ms

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_bEventHanding(false)
    , m_bCallingPendingFunctors(false)
    , m_threadId(CurrentThread::tid())
    , m_pPoller(new Poller(this))
    , m_pTimerQueue(new TimerQueue(this))
    , m_wakeupfd(createEventfd())
    , m_pWakeupChannel(new Channel(this, m_wakeupfd))
{
    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another event loop " << t_loopInThisThread << " exists in this thread " << m_threadId;
    }
    else
    {
        t_loopInThisThread = this;
    }

    m_pWakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead, this, _1));
    m_pWakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
    assert(!m_bLooping);
    m_pWakeupChannel->disableAll();
    m_pWakeupChannel->remove();
    ::close(m_wakeupfd);

    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    assertInLoopThread();
    m_bLooping = true;
    m_bQuit = false;

    while (m_bQuit == false)
    {
        m_activeChannels.clear();
        m_pollRetrunTime = m_pPoller->poll(kPollTimeMs, &m_activeChannels);

        m_bEventHanding = true;
        for (Channel * pItem : m_activeChannels)
        {
            m_pCurrentActiveChannel = pItem;
            m_pCurrentActiveChannel->handleEvent(m_pollRetrunTime);
        }
        m_pCurrentActiveChannel = nullptr;
        m_bEventHanding = false;

        doPendingFunctors();
    }

    m_bLooping = false;
}

void EventLoop::quit()
{
    m_bQuit = true;
    if (!isInLoopThread())
    {
        wakeup();
    }
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

void EventLoop::removeChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    if (m_bEventHanding)
    {
        assert(m_pCurrentActiveChannel == pChannel || std::find(m_activeChannels.begin(), m_activeChannels.end(), pChannel) == m_activeChannels.end());
    }
    m_pPoller->removeChannel(pChannel);
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

    for (const Functors & item : vecFunctors)
    {
        item();
    }

    m_bCallingPendingFunctors = false;
}
