#ifndef EVETNLOOP_THREAD_H_
#define EVETNLOOP_THREAD_H_

#include "Condition.h"
#include "Mutex.h"
#include "NonCopyable.h"
#include "Thread.h"

class EventLoop;
class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop * startLoop();

private:
    void threadFunc();

private:
    EventLoop * m_pLoop GUARDED_BY(m_mutex);
    Thread m_thread;
    MutexLock m_mutex;
    Condition m_cond GUARDED_BY(m_mutex);
};

#endif