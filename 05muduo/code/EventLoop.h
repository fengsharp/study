#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include "Thread.h"
#include "CurrentThread.h"

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const 
    {
        return m_threadId == CurrentThread::tid();
    }
private:
    void abortNotInLoopThread();
private:
    bool m_bLooping;
    const pid_t m_threadId;
};

#endif