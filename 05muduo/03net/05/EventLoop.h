#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include "NonCopyable.h"

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    
private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
};

#endif