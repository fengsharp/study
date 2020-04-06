#include "Thread.h"

#include <stdio.h>

#include <type_traits>
#include <exception>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "Exception.h"

struct ThreadData
{
    Thread::ThreadFunc m_threadCallback;
    std::string m_name;
    pid_t* m_pTid;
    CountDownLatch* m_latch;

    ThreadData(Thread::ThreadFunc func,
                const std::string& name,
                pid_t* tid,
                CountDownLatch* latch)
        : m_threadCallback(std::move(func)),
        m_name(name),
        m_pTid(tid),
        m_latch(latch)
    { }

    void runInThread()
    {
        *m_pTid = CurrentThread::tid();
        m_pTid = NULL;
        m_latch->countDown();
        m_latch = NULL;

        CurrentThread::t_threadName = m_name.empty() ? "muduoThread" : m_name.c_str();
        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
        try
        {
            m_threadCallback();
            CurrentThread::t_threadName = "finished";
        }
        catch (const Exception& ex)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        }
        catch (const std::exception& ex)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", m_name.c_str());
            throw; // rethrow
        }
    }
};

void* startThread(void* obj)
{
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

AtomicInt32 Thread::m_numCreated;

Thread::Thread(ThreadFunc func, const std::string& n)
  : m_bStart(false),
    m_bJoin(false),
    m_pthreadId(0),
    m_tid(0),
    m_funcCallback(std::move(func)),
    m_name(n),
    m_latch(1)
{
  setDefaultName();
}

Thread::~Thread()
{
    if (m_bStart && !m_bJoin)
    {
        pthread_detach(m_pthreadId);
    }
}

void Thread::setDefaultName()
{
    int num = m_numCreated.incrementAndGet();
    if (m_name.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        m_name = buf;
    }
}

void Thread::start()
{
    assert(!m_bStart);
    m_bStart = true;
    ThreadData* data = new ThreadData(m_funcCallback, m_name, &m_tid, &m_latch);
    if (pthread_create(&m_pthreadId, NULL, &startThread, data) != 0)
    {
        m_bStart = false;
        delete data; 
    }
    else
    {
        m_latch.wait();
        assert(m_tid > 0);
    }
}

int Thread::join()
{
    assert(m_bStart);
    assert(!m_bJoin);
    m_bJoin = true;
    return pthread_join(m_pthreadId, NULL);
}
