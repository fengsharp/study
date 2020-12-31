#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include <set>

#include "Callbacks.h"
#include "Channel.h"
#include "NonCopyable.h"
#include "Timestamp.h"

class TimerId;
class Timer : private NonCopyable
{
public:
    Timer(const TimerCallback & cb, Timestamp when, double interval)
        : m_callback(cb)
        , m_expiration(when)
        , m_interval(interval)
        , m_bRepeat(m_interval > 0.0)
    {
    }

    void run() const
    {
        m_callback();
    }

    Timestamp expiration() const
    {
        return m_expiration;
    }

    bool repeat() const
    {
        return m_bRepeat;
    }

    void restart(Timestamp now)
    {
        if (m_bRepeat)
        {
            m_expiration = addTime(m_expiration, m_interval);
        }
        else
        {
            m_expiration = Timestamp::invalid();
        }
    }

private:
    const TimerCallback m_callback;
    Timestamp m_expiration;
    const double m_interval;
    const bool m_bRepeat;
};

class EventLoop;
class TimerQueue : private NonCopyable
{
public:
    TimerQueue(EventLoop * pLoop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback & cb, Timestamp when, double interval);

private:
    void handleRead(Timestamp time);

    typedef std::pair<Timestamp, Timer *> Entry;
    std::vector<TimerQueue::Entry> getExpired(Timestamp now);
private:
    EventLoop * m_pLoop;
    const int m_timerfd;
    Channel m_timerChannel;

    std::set<Entry> m_setTimers;
};

#endif