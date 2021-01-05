#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include "NonCopyable.h"
#include "Poller.h"

class Channel;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    bool isLoopInThread() const;
    void assertLoopInThread();

    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);
private:
    void wakeup();
private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;

    Poller m_poller;
};

#endif