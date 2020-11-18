#ifndef EVENT_LOOP_THREAD_H_
#define EVENT_LOOP_THREAD_H_

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

class EventLoop;

class EventLoopThread : private NoneCopyable
{
public:
    typedef std::function<void(EventLoop *)> ThreadInitCallback;

public:
    EventLoopThread(const ThreadInitCallback & cb = ThreadInitCallback(), const string & name = string());
    ~EventLoopThread();

    EventLoop * startLoop();

private:
    void threadFunc();

private:
    EventLoop * m_pLoop GUARDED_BY(m_mutex);

    bool m_bExiting;
    Thread m_thread;
    MutexLock m_mutex;
    Condition m_cond GUARDED_BY(m_mutex);
    ThreadInitCallback m_initCallback;
};

#endif