#ifndef EVETN_LOOP_H_
#define EVETN_LOOP_H_

#include <pthread.h>

#include "Callbacks.h"
#include "NonCopyable.h"
#include "Poller.h"
#include "TimerQueue.h"
#include "Timestamp.h"
#include "EventWakeQueue.h"

class Channel;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    bool isLoopInThread() const;
    void assertLoopInthread();

    void updateChannel(Channel * pChannel);

    void runAt(Timestamp when, const TimerCallback & cb);
    void runAfter(double delay, const TimerCallback & cb);
    void runEvery(double interval, const TimerCallback & cb);

    void runInLoop(const LoopFunctor & cb);
    void queueInLoop(const LoopFunctor & cb);
private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;

    Poller m_poller;
    TimerQueue m_timer;
    EventWakeQueue m_eventWake;
};

#endif