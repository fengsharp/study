#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <memory>
#include <vector>

#include "CurrentThread.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Callbacks.h"
#include "TimerId.h"

enum PollerState
{
    NEW = 0,
    ADDED = 1,
    DELETED = 2
};

class Channel;
class Poller;
class TimerQueue;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    ///
    /// Runs callback at 'time'.
    /// Safe to call from other threads.
    ///
    TimerId runAt(const Timestamp & time, const TimerCallback & cb);
    ///
    /// Runs callback after @c delay seconds.
    /// Safe to call from other threads.
    ///
    TimerId runAfter(double delay, const TimerCallback & cb);
    ///
    /// Runs callback every @c interval seconds.
    /// Safe to call from other threads.
    ///
    TimerId runEvery(double interval, const TimerCallback & cb);
    ///
    /// Cancels the timer.
    /// Safe to call from other threads.
    ///
    void cancel(TimerId timerId);

    void updateChannel(Channel * pChannel);

    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const
    {
        return m_threadId == CurrentThread::tid();
    }

private:
    void abortNotInLoopThread();

private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
    Timestamp m_pollRetrunTime;
    std::unique_ptr<Poller> m_pPoller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;
    std::vector<Channel *> m_activeChannels;
};

#endif