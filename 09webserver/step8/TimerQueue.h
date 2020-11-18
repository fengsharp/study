#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include <set>
#include <vector>

#include "Callbacks.h"
#include "Channel.h"
#include "Mutex.h"
#include "Timestamp.h"

class EventLoop;
class Timer;
class TimerId;

class TimerQueue : private NoneCopyable
{
public:
    explicit TimerQueue(EventLoop * loop);
    ~TimerQueue();

    ///
    /// Schedules the callback to be run at given time,
    /// repeats if @c interval > 0.0.
    ///
    /// Must be thread safe. Usually be called from other threads.
    TimerId addTimer(TimerCallback cb,
                     Timestamp when,
                     double interval);

    void cancel(TimerId timerId);

private:
    typedef std::pair<Timestamp, Timer *> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer *, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer * timer);
    void cancelInLoop(TimerId timerId);
    // called when timerfd alarms
    void handleRead();
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry> & expired, Timestamp now);

    bool insert(Timer * timer);

private:
    EventLoop * loop_;
    const int timerfd_;
    Channel timerfdChannel_;

    // Timer list sorted by expiration
    TimerList timers_;

    // for cancel()
    ActiveTimerSet activeTimers_;
    bool callingExpiredTimers_; /* atomic */
    ActiveTimerSet cancelingTimers_;
};

#endif