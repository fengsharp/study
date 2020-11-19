#include "Timer.h"

AtomicInt64 Timer::s_numCreated;

void Timer::restart(Timestamp now)
{
    if (m_bReapeat)
    {
        m_expiration = addTime(now, m_interval);
    }
    else
    {
        m_expiration = Timestamp::invalid();
    }
}