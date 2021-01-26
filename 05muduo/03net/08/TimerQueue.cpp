#include "TimerQueue.h"

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "Types.h"

TimerQueue::TimerQueue(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_timerfd(::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC))
    , m_timerChannel(m_pLoop, m_timerfd)
{
    m_timerChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this));
    m_timerChannel.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(m_timerfd);
}

void TimerQueue::addTimer(const TimerCallback & cb, Timestamp when, double interval)
{
    m_callback = cb;

    int64_t microseconds = when.microSeconds() - Timestamp::now().microSeconds();
    if (microseconds < 100)
    {
        microseconds = 100;
    }

    struct itimerspec howlong;
    memZero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSeconds);
    howlong.it_value.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSeconds) * 1000);

    ::timerfd_settime(m_timerfd, 0, &howlong, NULL);
}

void TimerQueue::handleRead()
{
    if (m_callback)
    {
        m_callback();
    }
}