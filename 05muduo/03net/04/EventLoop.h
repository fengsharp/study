#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include "Callbacks.h"
#include "NonCopyable.h"
#include "Poller.h"
#include "TimerQueue.h"
#include "Timestamp.h"
#include "TimerId.h"

class Channel;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    bool isLoopInThread();
    void assertLoopInThread();
    void updateChannel(Channel * pChannel);

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