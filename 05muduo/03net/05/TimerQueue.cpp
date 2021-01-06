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

void setTimer(int timerfd, Timestamp when)
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

void readTimer(int timerfd)
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
    
    m_delCallback = cb;
    setTimer(m_timerfd, when);
}

void TimerQueue::handleRead(Timestamp received)
{
    readTimer(m_timerfd);

    if (m_delCallback)
    {
        m_delCallback();
    }
}