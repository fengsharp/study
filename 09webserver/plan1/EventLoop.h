#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <vector>

#include "CurrentThread.h"
#include "NoneCopyable.h"

class EventLoop : private NoneCopyable
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
        return m_threadId == CurrentThread::gettid();
    }

    static EventLoop * getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();

private:
    bool m_bLooping;
    const pid_t m_threadId; // 当前对象所属线程ID
};

#endif