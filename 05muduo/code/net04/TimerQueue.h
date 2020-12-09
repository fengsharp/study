#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include <set>
#include <vector>

#include "Callbacks.h"
#include "Channel.h"
#include "Mutex.h"
#include "Noncopyable.h"
#include "Timestamp.h"

class EventLoop;
class Timer;
class TimerId;

class TimerQueue : private NonCopyable
{
public:
    explicit TimerQueue(EventLoop * pLoop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback & cb, Timestamp when, double interval);

private:
    typedef std::pair<Timestamp, Timer *> Entry;
    typedef std::set<Entry> TimerList;

    void addTimerInLoop(Timer * pTimer);
    
    void handleRead(Timestamp);
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry> & expired, Timestamp now);

    bool insert(Timer * pTimer);

private:
    EventLoop * m_pLoop;
    const int m_timerfd;
    Channel m_timerfdChannel;

    TimerList m_timers;
};

#endif