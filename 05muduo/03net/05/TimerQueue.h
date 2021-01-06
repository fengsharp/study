#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include <set>
#include <memory>

#include "Callbacks.h"
#include "Channel.h"
#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;

class Timer
{
public:
    Timer(const TimerCallback & cb, Timestamp when, double interval)
        : m_timerCallback(cb)
        , m_when(when)
        , m_interval(interval)
    {
    }

    void run()
    {
        if (m_timerCallback)
        {
            m_timerCallback();
        }
    }

    Timestamp expiration()
    {
        return m_when;
    }

    bool repeat() const
    {
        return m_interval > 0.0;
    }

    void restart(Timestamp now)
    {
        if (repeat())
        {
            m_when = addTime(now, m_interval);
        }
        else
        {
            m_when = Timestamp::invalid();
        }
    }
private:
    TimerCallback m_timerCallback;
    Timestamp m_when;
    double m_interval;
};

class TimerQueue
{
public:
    explicit TimerQueue(EventLoop * pLoop);
    ~TimerQueue();

    void addTimer(const TimerCallback & cb, Timestamp when, double interval);

private:
    void handleRead(Timestamp received);
    bool insert(const TimerCallback & cb, Timestamp when, double interval);
    typedef std::pair<Timestamp, std::shared_ptr<Timer>> TimerEntry;
    std::vector<TimerEntry> getExpired(Timestamp now);
private:
    EventLoop * m_pLoop;
    int m_timerfd;
    Channel m_timerChannel;

    std::set<TimerEntry> m_setTimers;

};

#endif