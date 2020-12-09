#include "TimerQueue.h"

#include <functional>

#include <sys/timerfd.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Logging.h"
#include "Timer.h"
#include "TimerId.h"

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_FATAL << "failed in timerfd_create()";
    }
    return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicrosecondsPerSeconds);
    ts.tv_nsec = static_cast<time_t>(microseconds % Timestamp::kMicrosecondsPerSeconds * 1000);
    return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    if (n != sizeof(howmany))
    {
        LOG_ERROR << "readTimerfd " << n << " bytes instead of 8";
    }
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
    struct itimerspec newValue;
    bzero(&newValue, sizeof(newValue));

    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, NULL);
    if (ret != 0)
    {
        LOG_SYSERR << "timerfd settime";
    }
}

TimerQueue::TimerQueue(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_timerfd(createTimerfd())
    , m_timerfdChannel(pLoop, m_timerfd)
    , m_timers()
{
    m_timerfdChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this, std::placeholders::_1));
    m_timerfdChannel.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(m_timerfd);

    for (auto & item : m_timers)
    {
        delete item.second;
    }
}

TimerId TimerQueue::addTimer(const TimerCallback & cb, Timestamp when, double interval)
{
    Timer * pTimer = new Timer(cb, when, interval);
    m_pLoop->assertLoopInThisThread();
    bool earlistChaned = insert(pTimer);

    if (earlistChaned)
    {
        resetTimerfd(m_timerfd, pTimer->expiration());
    }
    return TimerId(pTimer);
}

void TimerQueue::handleRead(Timestamp receiveTime)
{
    m_pLoop->assertLoopInThisThread();
    Timestamp now(Timestamp::now());
    readTimerfd(m_timerfd, now);

    std::vector<Entry> expired = getExpired(now);
    for (auto & item : expired)
    {
        item.second->run();
    }
    reset(expired, now);
}

std::vector<std::pair<Timestamp, Timer *>> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator it = m_timers.lower_bound(sentry);
    assert(it == m_timers.end() || now < it->first);
    std::copy(m_timers.begin(), it, std::back_inserter(expired));
    m_timers.erase(m_timers.begin(), it);

    return expired;
}

void TimerQueue::reset(const std::vector<Entry> & expired, Timestamp now)
{
    Timestamp nextExpire;

    for (std::vector<Entry>::const_iterator it = expired.begin(); it != expired.end(); ++it)
    {
        if (it->second->repeat())
        {
            it->second->restart(now);
            insert(it->second);
        }
        else
        {
            delete it->second;
        }
    }

    if (!m_timers.empty())
    {
        nextExpire = m_timers.begin()->second->expiration();
    }

    if (nextExpire.valid())
    {
        resetTimerfd(m_timerfd, nextExpire);
    }
}

bool TimerQueue::insert(Timer * pTimer)
{
    bool earlistChanged = false;
    Timestamp when = pTimer->expiration();
    TimerList::iterator it = m_timers.begin();
    if (it == m_timers.end() || when < it->first)
    {
        earlistChanged = true;
    }
    std::pair<TimerList::iterator, bool> result = m_timers.insert(std::make_pair(when, pTimer));
    assert(result.second);
    return earlistChanged;
}