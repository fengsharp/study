#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include "Callbacks.h"
#include "Channel.h"
#include "NonCopyable.h"

class EventLoop;

class TimerQueue : private NonCopyable
{
public:
    explicit TimerQueue(EventLoop * pLoop);
    ~TimerQueue();

    void addTimer(const TimerCallback & cb, Timestamp when, double interval);

private:
    void handleRead();
private:
    EventLoop * m_pLoop;
    int m_timerfd;
    Channel m_timerChannel;

    TimerCallback m_callback;
};

#endif