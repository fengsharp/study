#ifndef TIMER_H_
#define TIMER_H_

#include "Atomic.h"
#include "Callbacks.h"
#include "Timestamp.h"

class Timer : private NonCopyable
{
public:
    Timer(TimerCallback cb, Timestamp when, double interval)
        : m_callback(cb)
        , m_expiration(when)
        , m_interval(interval)
        , m_bReapeat(interval > 0.0)
        , m_sequence(s_numCreated.incrementAndGet())
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
        return m_bReapeat;
    }

    int64_t sequence() const
    {
        return m_sequence;
    }

    void restart(Timestamp now);

    static int64_t numCreated()
    {
        return s_numCreated.get();
    }

private:
    const TimerCallback m_callback;
    Timestamp m_expiration;
    const double m_interval;
    const bool m_bReapeat;
    const int64_t m_sequence;

    static AtomicInt64 s_numCreated;
};

#endif