#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include "NonCopyable.h"
#include "Poller.h"
#include "Callbacks.h"
#include "TimerQueue.h"

class Channel;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    bool isLoopInThread() const;
    void assertLoopInThread() const;

    void updateChannel(Channel * pChannel);

    void runAt(const Timestamp & time, const TimerCallback & cb);
    void runAfter(double delay, const TimerCallback & cb);
    void runEvery(double interval, const TimerCallback & cb);

private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;

    Poller m_poller;
    TimerQueue m_timerQueue;
};

#endif