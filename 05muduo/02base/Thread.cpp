#include "Thread.h"

#include "CurrentThread.h"

AtomicInt32 Thread::m_numCreated;

Thread::Thread(const ThreadFunc & func, const std::string & name)
    : m_func(func)
    , m_strName(name)
    , m_bStarted(false)
    , m_bJoined(false)
    , m_pthreadId(0)
    , m_tid(0)
    , m_latch(1)
{
    m_numCreated.increment();
}

Thread::~Thread()
{
    if (m_bStarted && !m_bJoined)
    {
        pthread_detach(m_pthreadId);
    }
}

void Thread::start()
{
    assert(m_bStarted == false);
    m_bStarted = true;
    pthread_create(&m_pthreadId, NULL, &Thread::thread_work, this);
    m_latch.wait();
}

void Thread::join()
{
    assert(m_bStarted == true);
    assert(m_bJoined == false);
    m_bJoined = true;

    pthread_join(m_pthreadId, NULL);
}

void * Thread::thread_work(void * args)
{
    Thread * pThread = static_cast<Thread *>(args);
    pThread->m_tid = CurrentThread::tid();
    CurrentThread::t_threadName = pThread->m_strName.data();

    pThread->m_latch.countDown();
    pThread->m_func();

    return NULL;
}