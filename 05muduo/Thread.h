#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <string>

#include <functional>
#include <memory>
#include <pthread.h>

#include "noncopyable.h"
#include "Atomic.h"
#include "CountDownLatch.h"
#include "CurrentThread.h"

class Thread : noncopyable
{
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(ThreadFunc, const std::string& name = std::string());
    ~Thread();

    void start();
    int join();

    bool started() const { return m_bStart; }
    pid_t tid() const { return m_tid; }
    const std::string& name() const { return m_name; }

    static int numCreated() { return m_numCreated.get(); }
private:
    void setDefaultName();

    bool       m_bStart;
    bool       m_bJoin;
    pthread_t  m_pthreadId;
    pid_t      m_tid;
    ThreadFunc m_funcCallback;
    std::string     m_name;
    CountDownLatch m_latch;

    static AtomicInt32 m_numCreated;
};

#endif