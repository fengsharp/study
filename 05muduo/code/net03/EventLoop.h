#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <memory>
#include <vector>

#include "Callbacks.h"
#include "CurrentThread.h"
#include "Noncopyable.h"
#include "TimerId.h"
#include "Timestamp.h"

class Channel;
class Poller;
class TimerQueue;
class WeakupQueue;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    TimerId runAt(const Timestamp & time, const TimerCallback & cb);
    TimerId runAfter(double delay, const TimerCallback & cb);
    TimerId runEvery(double inteterval, const TimerCallback & cb);

    bool hasChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);
    void updateChannel(Channel * pChannel);

    bool isInLoopThread() const
    {
        return m_threadId == CurrentThread::tid();
    }

    void assertLoopInThisThread() const
    {
        assert(isInLoopThread() == true);
    }

    void runInLoop(const Functor & cb);
    void queueInLoop(const Functor & cb);

private:
    bool m_bLooping;
    bool m_bQuit;
    bool m_bEventHanding;
    pid_t m_threadId;
    Timestamp m_pollReturnTime;
    std::unique_ptr<Poller> m_pPoller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;

    std::vector<Channel *> m_vecActiveChannels;
    Channel * m_pCurrentActiveChannel;

    std::unique_ptr<WeakupQueue> m_pWeakupQueue;
};

#endif