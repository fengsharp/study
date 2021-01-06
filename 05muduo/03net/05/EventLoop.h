#ifndef EVETN_LOOP_H_
#define EVETN_LOOP_H_

#include <pthread.h>
#include "NonCopyable.h"

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();

    bool isLoopInThread() const;
    void assertLoopInthread();

private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
};

#endif