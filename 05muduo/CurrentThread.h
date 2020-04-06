#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include <string>
#include <assert.h>

namespace CurrentThread
{
    // demangle: C++编译以后的函数名
    std::string stackTrace(bool demangle);

    // internal
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadName;
    
    void cacheTid();

    inline int tid() {
        if (t_cachedTid == 0) {
            cacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString() {
        return t_tidString;
    }

    inline int tidStringLength() {
        return t_tidStringLength;
    }

    inline const char* name() {
        return t_threadName;
    }

    bool isMainThread();
    void sleepUsec(int64_t usec);

    pid_t gettid();
}

#endif