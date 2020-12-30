#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include "NonCopyable.h"

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    bool isLoopInThread();
    void assertInLoopThread();

private:
    bool m_bLooping;
    const pid_t m_threadId;
};

#endif