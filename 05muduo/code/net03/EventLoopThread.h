#ifndef EVETN_LOOP_THREAD_H_
#define EVETN_LOOP_THREAD_H_

#include "Noncopyable.h"
#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

class EventLoop;

class EvnetLoopThread : private NonCopyable
{
public:
    EvnetLoopThread();
    ~EvnetLoopThread();
    EventLoop * startLoop();

private:
    void threadFunc();
private:
    EventLoop * m_pLoop;
    bool m_bExiting;
    Thread m_thread;
    MutexLock m_mutex;
    Condition m_cond;
};

#endif