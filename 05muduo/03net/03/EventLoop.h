#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include <vector>

#include "NonCopyable.h"

#include "Poller.h"

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
private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
    Poller m_poller;

};

#endif