#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include <vector>

#include "NonCopyable.h"

#include "Callbacks.h"
#include "Poller.h"
#include "TimerQueue.h"
#include "TimerId.h"

class Channel;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    void updateChannel(Channel * pChannel);

    bool isInLoopThread();
    void assertInLoopThread();

    TimerId runAt(const Timestamp & time, const TimerCallback & cb);
    TimerId runAfter(double delay, const TimerCallback & cb);
    TimerId runEvery(double interval, const TimerCallback & cb);
private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
    Poller m_poller;
    TimerQueue m_timerQueue;
};

#endif