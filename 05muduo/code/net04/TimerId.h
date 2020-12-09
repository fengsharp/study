#ifndef TIMER_ID_H_
#define TIMER_ID_H_

#include "Copyable.h"

class Timer;

class TimerId : public Copyable
{
public:
    explicit TimerId(Timer * pTimer)
        : m_pTimer(pTimer)
    {
        (void)m_pTimer;
    }

private:
    Timer * m_pTimer;
};

#endif