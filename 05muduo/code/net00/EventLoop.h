#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include "Noncopyable.h"
#include "Thread.h"
#include "CurrentThread.h"

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    bool isInLoopThread() const
    {
        return m_threadId == CurrentThread::tid();
    }

    void assertInLoopThread()
    {
        if (isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

private:
    void abortNotInLoopThread();

private:
    bool m_bLooping;
    const pid_t m_threadId;
};

#endif