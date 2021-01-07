#ifndef EVETN_LOOP_THREAD_H_
#define EVETN_LOOP_THREAD_H_

#include "NonCopyable.h"
#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"

class EventLoop;

class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop * startLoop();
private:
    void threadCallback();
    
private:
    EventLoop * m_pLoop GUARDED_BY(m_mutex);
    MutexLock m_mutex;
    Condition m_cond GUARDED_BY(m_mutex);

    Thread m_thread;
};

#endif