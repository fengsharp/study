#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include "NonCopyable.h"

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    bool isLoopInThread() const;
    void assertLoopInThread();
private:
    void wakeup();
private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
};

#endif