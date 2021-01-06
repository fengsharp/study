#include "TimerQueue.h"

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Types.h"

int createTimerfd()
{
    int ret = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(ret >= 0);
    return ret;
}

void setTimerfd(int timerfd, Timestamp when)
{
    int64_t microseconds = when.microSeconds() - Timestamp::now().microSeconds();
    if (microseconds < 100)
    {
        microseconds = 100;
    }

    struct itimerspec itspec;
    memZero(&itspec, sizeof(itspec));
    itspec.it_value.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSeconds);
    itspec.it_value.tv_nsec = static_cast<long>(microseconds % Timestamp::kMicroSecondsPerSeconds * 1000);
    int ret = timerfd_settime(timerfd, 0, &itspec, NULL);
    assert(ret == 0);
}

void readTimerfd(int timerfd)
{
    int64_t howmany = 0;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    assert(n == sizeof(howmany));
}

TimerQueue::TimerQueue(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_timerfd(createTimerfd())
    , m_timerChannel(m_pLoop, m_timerfd)
{
    m_timerChannel.setEventReadCallback(std::bind(&TimerQueue::handleRead, this, std::placeholders::_1));
    m_timerChannel.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(m_timerfd);
}

void TimerQueue::addTimer(const TimerCallback & cb, Timestamp when, double interval)
{
    m_pLoop->assertLoopInthread();

    bool earliesChanged = insert(cb, when, interval);
    if (earliesChanged)
    {
        setTimerfd(m_timerfd, when);
    }
}

void TimerQueue::handleRead(Timestamp received)
{
    m_pLoop->assertLoopInthread();
    readTimerfd(m_timerfd);

    Timestamp now(Timestamp::now());
    std::vector<TimerEntry> expiredTimers = getExpired(now);

    for (auto & item : expiredTimers)
    {
        item.second->run();
    }

    for (auto & item : expiredTimers)
    {
        if (item.second->repeat())
        {
            item.second->restart(now);
            m_setTimers.insert(std::make_pair(now, item.second));
        }
    }

    if (m_setTimers.empty() == false)
    {
        setTimerfd(m_timerfd, m_setTimers.begin()->second->expiration());
    }
}

bool TimerQueue::insert(const TimerCallback & cb, Timestamp when, double interval)
{
    bool earliesChanged = false;

    std::set<TimerEntry>::const_iterator begin = m_setTimers.begin();
    if (begin == m_setTimers.end() || when < begin->first)
    {
        earliesChanged = true;
    }

    m_setTimers.insert(std::make_pair(when, std::make_shared<Timer>(cb, when, interval)));

    return earliesChanged;
}

std::vector<TimerQueue::TimerEntry> TimerQueue::getExpired(Timestamp now)
{
    assert(m_setTimers.empty() == false);
    
    std::vector<TimerEntry> vecRet;
    std::set<TimerEntry>::iterator findItem = std::find_if(m_setTimers.begin(), m_setTimers.end(), [now](const std::set<TimerEntry>::value_type & item) {
        return now < item.first;
    });

    std::copy(m_setTimers.begin(), findItem, std::back_inserter(vecRet));
    m_setTimers.erase(m_setTimers.begin(), findItem);

    return vecRet;
}