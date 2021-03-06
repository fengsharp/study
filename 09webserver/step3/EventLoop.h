#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include "CurrentThread.h"
#include "Mutex.h"
#include "Timestamp.h"
#include "Callbacks.h"
#include "TimerId.h"

class Channel;
class Poller;
class TimerQueue;

class EventLoop : private NoneCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    Timestamp pollReturnTime() const
    {
        return m_pollRetrunTime;
    }

    bool hasChannel(Channel * pChannel);
    void updateChannel(Channel * pChannel); // 在Poller中添加或者更新通道
    void removeChannel(Channel * pChannel); // 从Poller中移除通道

    void assertInLoopThread();
    bool isInLoopThread();

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

private:
    bool m_bLooping; // atomic
    std::atomic<bool> m_bQuit;
    bool m_bEventHanding; // atomic
    const pid_t m_threadId;
    Timestamp m_pollRetrunTime;
    std::unique_ptr<Poller> m_pPoller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;
    std::vector<Channel *> m_vecActiveChannels;
    Channel * m_pCurrentActiveChannel;
};

#endif