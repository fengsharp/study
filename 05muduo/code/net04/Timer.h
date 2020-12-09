#ifndef TIMER_H_
#define TIMER_H_

#include "Callbacks.h"
#include "Noncopyable.h"
#include "Timestamp.h"

class Timer : private NonCopyable
{
public:
    Timer(const TimerCallback & cb, Timestamp when, double interval)
        : m_timerCallback(cb)
        , m_expiration(when)
        , m_interver(interval)
        , m_bRepeat(interval > 0.0)
    {
    }

    void run() const
    {
        m_timerCallback();
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
            m_expiration = addTime(now, m_interver);
        }
        else
        {
            m_expiration = Timestamp::invalid();
        }
    }

private:
    const TimerCallback m_timerCallback;
    Timestamp m_expiration;
    const double m_interver;
    const bool m_bRepeat;
};

#endif