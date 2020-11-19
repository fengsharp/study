#ifndef TIMER_ID_H_
#define TIMER_ID_H_

#include <stdint.h>

#include "Copyable.h"

class Timer;

class TimerId : public Copyable
{
public:
    TimerId()
        : m_pTimer(nullptr)
        , m_sequence(0)
    {
    }

    TimerId(Timer * pTimer, int64_t seq)
        : m_pTimer(pTimer)
        , m_sequence(seq)
    {
    }

    friend class TimerQueue;

private:
    Timer * m_pTimer;
    int64_t m_sequence;
};

#endif