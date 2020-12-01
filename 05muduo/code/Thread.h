#ifndef THREAD_H_
#define THREAD_H_

#include <functional>
#include <memory>
#include <pthread.h>

#include "Noncopyable.h"
#include "Types.h"

class Thread : private NonCopyable
{
public:
    typedef std::function<void()> ThreadFunc;

    Thread(ThreadFunc func, const std::string & name);
    ~Thread();

    void start();
    void join();

private:
    std::string m_strName;
    ThreadFunc m_func;
    pthread_t m_threadId;
    bool m_bStarted;
    bool m_bJoined;
    
};

#endif