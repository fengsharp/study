#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include "Channel.h"
#include "NonCopyable.h"
#include "Timestamp.h"
#include "Callbacks.h"

class EventLoop;

class TimerQueue
{
public:
    explicit TimerQueue(EventLoop * pLoop);
    ~TimerQueue();

    void addTimer(const TimerCallback & cb, Timestamp when, double interval);
private:
    void handleRead(Timestamp received);
private:
    EventLoop * m_pLoop;
    int m_timerfd;
    Channel m_timerChannel;

    TimerCallback m_delCallback;
};

#endif