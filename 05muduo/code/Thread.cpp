#include "Thread.h"

#include <assert.h>

#include "CurrentThread.h"

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        CurrentThread::cacheTid();
        CurrentThread::t_threadName = "main";
    }
};

struct ThreadArgs
{
public:
    ThreadArgs(Thread::ThreadFunc func, const std::string & name)
        : m_func(std::move(func))
        , m_strName(name)
    {
    }

    void runInThread()
    {
        CurrentThread::cacheTid();
        CurrentThread::t_threadName = m_strName.empty() ? "default_thread" : m_strName.data();

        m_func();
    }

private:
    Thread::ThreadFunc m_func;
    std::string m_strName;
};

void * runThread(void * args)
{
    ThreadArgs * pArgs = static_cast<ThreadArgs *>(args);
    pArgs->runInThread();

    delete pArgs;
    return NULL;
}

Thread::Thread(ThreadFunc func, const std::string & name)
    : m_strName(name)
    , m_func(std::move(func))
    , m_bStarted(false)
    , m_bJoined(false)
{
}

Thread::~Thread()
{
    if (m_bStarted && !m_bJoined)
    {
        pthread_detach(m_threadId);
    }
}

void Thread::start()
{
    assert(m_bStarted == false);
    m_bStarted = true;

    ThreadArgs * pArgs = new ThreadArgs(m_func, m_strName);
    if (pthread_create(&m_threadId, NULL, runThread, pArgs) < 0)
    {
        m_bStarted = false;
        delete pArgs;
    }
    else
    {
        assert(CurrentThread::tid() > 0);
    }
}

void Thread::join()
{
    assert(m_bStarted == true);
    assert(m_bJoined == false);

    m_bJoined = true;
    pthread_join(m_threadId, NULL);
}
