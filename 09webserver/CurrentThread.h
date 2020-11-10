#ifndef CURRENT_THREAD_H_
#define CURRENT_THREAD_H_

#include "Types.h"

namespace CurrentThread
{
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char * t_threadName;

pid_t gettid();

void cacheTid();

inline int tid()
{
    /*
        允许程序员将最有可能执行的分支告诉编译器 __builtin_expect(EXP, N)
        EXP==N的概率很大
    */
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
        cacheTid();
    }
    return t_cachedTid;
}

inline const char* tidString()
{
    return t_tidString;
}

inline int tidStringLength()
{
    return t_tidStringLength;
}

inline const char* name()
{
    return t_threadName;
}

bool isMainThread();
// for test usleep
void sleepUsec(int64_t usec);

string stackTrace(bool demangle);
} // namespace CurrentThread

#endif