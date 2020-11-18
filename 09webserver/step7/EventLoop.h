#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include "Callbacks.h"
#include "CurrentThread.h"
#include "Mutex.h"
#include "TimerId.h"
#include "Timestamp.h"

class Channel;
class Poller;
class TimerQueue;

class EventLoop : private NoneCopyable
{
public:
    typedef std::function<void()> Functors;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    Timestamp pollReturnTime() const
    {
        return m_pollRetrunTime;
    }

    void runInLoop(Functors cb);
    void queueInLoop(Functors cb);
    size_t queueSize() const;

    void wakeup();
    bool hasChannel(Channel * pChannel);
    void updateChannel(Channel * pChannel); // 在Poller中添加或者更新通道
    void removeChannel(Channel * pChannel); // 从Poller中移除通道

    void assertInLoopThread();
    bool isInLoopThread();
    bool eventHanding() const
    {
        return m_bEventHanding;
    }

    // timers

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

    static EventLoop * getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();
    void printActiveChannels() const; // DEBUG

    void handleRead(Timestamp receiveTime); // wake up
    void doPendingFunctors();

private:
    bool m_bLooping; // atomic
    std::atomic<bool> m_bQuit;
    bool m_bEventHanding; // atomic
    bool m_bCallingPendingFunctors;

    const pid_t m_threadId;
    Timestamp m_pollRetrunTime;
    std::unique_ptr<Poller> m_pPoller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;
    int m_wakeupfd;
    std::unique_ptr<Channel> m_pWakeupChannel;
    std::vector<Channel *> m_vecActiveChannels;
    Channel * m_pCurrentActiveChannel;

    mutable MutexLock m_mutex;
    std::vector<Functors> m_vecPendingFunctors GUARDED_BY(m_mutex);
};

#endif