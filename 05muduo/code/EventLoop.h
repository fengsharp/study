#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include "Callbacks.h"
#include "CurrentThread.h"
#include "Thread.h"
#include "TimerId.h"
#include "Timestamp.h"
#include "Mutex.h"

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
    typedef std::function<void()> Functors;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop(Functors cb);
    void queueInLoop(Functors cb);
    size_t queueSize() const;

    void wakeup();
    bool hasChannel(Channel * pChannel);
    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel); // 从Poller中移除通道

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

    static EventLoop * getEventLoopOfCurrentThread();
private:
    void abortNotInLoopThread();

    void handleRead(Timestamp receiveTime); // wake up
    void doPendingFunctors();

private:
    bool m_bLooping;
    bool m_bQuit;
    bool m_bEventHanding; // atomic
    bool m_bCallingPendingFunctors;

    const pid_t m_threadId;
    Timestamp m_pollRetrunTime;
    std::unique_ptr<Poller> m_pPoller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;
    int m_wakeupfd;
    std::unique_ptr<Channel> m_pWakeupChannel;
    std::vector<Channel *> m_activeChannels;
    Channel * m_pCurrentActiveChannel;

    mutable MutexLock m_mutex;
    std::vector<Functors> m_vecPendingFunctors GUARDED_BY(m_mutex);
};

#endif