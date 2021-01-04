#include "TimerQueue.h"

#include <assert.h>

#include <sys/timerfd.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Types.h"
#include "TimerId.h"

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSeconds() - Timestamp::now().microSeconds();
    if (microseconds < 100)
    {
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSeconds);
    ts.tv_nsec = static_cast<long>(microseconds % Timestamp::kMicroSecondsPerSeconds);
    return ts;
}

void readTimerfd(int timerfd)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    if (n != sizeof(howmany))
    {
        abort();
    }
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memZero(&newValue, sizeof(newValue));
    memZero(&oldValue, sizeof(oldValue));

    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    assert(ret == 0);
}

TimerQueue::TimerQueue(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_timerfd(::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC))
    , m_timerChannel(m_pLoop, m_timerfd)
{
    assert(m_timerfd > 0);
    m_timerChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this, std::placeholders::_1));
    m_timerChannel.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(m_timerfd);

    for (auto & item : m_setTimers)
    {
        delete item.second;
    }
}

TimerId TimerQueue::addTimer(const TimerCallback & cb, Timestamp when, double interval)
{
    m_pLoop->assertLoopInThread();

    Timer * pTimer = new Timer(cb, when, interval);
    Timestamp nowFirst = m_setTimers.empty() ? Timestamp::invalid() : m_setTimers.begin()->first;
    m_setTimers.insert(std::make_pair(when, pTimer));

    if (nowFirst.validate() == false || (when < nowFirst))
    {
        resetTimerfd(m_timerfd, when);
    }

    return TimerId(pTimer);
}

void TimerQueue::handleRead(Timestamp time)
{
    m_pLoop->assertLoopInThread();
    Timestamp now = Timestamp::now();
    readTimerfd(m_timerfd);

    std::vector<TimerQueue::Entry> expiredTimers = getExpired(now);
    for (auto & item : expiredTimers)
    {
        item.second->run();
    }

    for (auto & item : expiredTimers)
    {
        if (item.second->repeat())
        {
            item.second->restart(now);
            m_setTimers.insert(std::make_pair(item.second->expiration(), item.second));
        }
        else
        {
            delete item.second;
        }
    }

    if (!m_setTimers.empty())
    {
        Timestamp nextExpired = m_setTimers.begin()->second->expiration();
        if (nextExpired.validate())
        {
            resetTimerfd(m_timerfd, nextExpired);
        }
    }
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry sentry = std::make_pair(now, reinterpret_cast<Timer *>(UINTPTR_MAX));
    std::set<Entry>::iterator it = m_setTimers.lower_bound(sentry);
    std::copy(m_setTimers.begin(), it, std::back_inserter(expired));
    m_setTimers.erase(m_setTimers.begin(), it);

    return expired;
}