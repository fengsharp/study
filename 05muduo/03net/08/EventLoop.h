#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

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

    bool isLoopInThread() const;
    void assertLoopInThread() const;

    void update(Channel * pChannel);
private:
    bool m_bLooping;
    bool m_bQuite;
    const pid_t m_threadId;

    Poller m_poller;
};


#endif