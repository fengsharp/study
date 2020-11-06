#include "Thread.h"

#include <errno.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <type_traits>

#include "CurrentThread.h"
#include "Exception.h"

void afterFork()
{
    CurrentThread::t_cachedTid = 0;
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

ThreadNameInitializer init;

struct ThreadData
{
    std::function<void()> m_funcCallback;
    string m_strName;
    pid_t * m_pTid;
    CountDownLatch * m_pLatch;

    ThreadData(std::function<void()> funcCallback, const string & name, pid_t * pTid, CountDownLatch * pLatch)
        : m_funcCallback(funcCallback)
        , m_strName(name)
        , m_pTid(pTid)
        , m_pLatch(pLatch)
    {
    }

    void runInThread()
    {
        *m_pTid = CurrentThread::tid();
        m_pTid = nullptr;
        m_pLatch->countDown();
        m_pLatch = nullptr;

        CurrentThread::t_threadName = m_strName.empty() ? "myThread" : m_strName.c_str();
        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);

        try
        {
            m_funcCallback();
            CurrentThread::t_threadName = "finished";
        }
        catch (const Exception & ex)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", m_strName.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        }
        catch (const std::exception & ex)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", m_strName.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", m_strName.c_str());
            throw; // rethrow
        }
    }
};

void * startThread(void * args)
{
    ThreadData * pData = static_cast<ThreadData *>(args);
    pData->runInThread();
    delete pData;
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AtomicInt32 Thread::m_numCreated;

Thread::Thread(std::function<void()> funCallback, const string & name /* = string()*/)
    : m_fnThreadCallback(std::move(funCallback))
    , m_bStarted(false)
    , m_bJoined(false)
    , m_pthreadId(0)
    , m_tid(0)
    , m_strName(name)
    , m_latch(1)
{
    setDefaultName();
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
    assert(!m_bStarted);
    m_bStarted = true;
    ThreadData * pData = new ThreadData(m_fnThreadCallback, m_strName, &m_tid, &m_latch);
    if (pthread_create(&m_pthreadId, NULL, &startThread, pData))
    {
        m_bStarted = false;
        delete pData;
        // failed in pthread_create
    }
    else // pthread_create return 0
    {
        m_latch.wait();
        assert(m_tid > 0);
    }
}

int Thread::join()
{
    assert(m_bStarted);
    assert(!m_bJoined);
    m_bJoined = true;
    return pthread_join(m_pthreadId, NULL);
}

void Thread::setDefaultName()
{
    int num = m_numCreated.incrementAndGet();
    if (m_strName.empty())
    {
        char buf[32] = { 0 };
        snprintf(buf, sizeof(buf) - 1, "Thread%d", num);
        m_strName = buf;
    }
}