#ifndef CURRENT_THREAD_H_
#define CURRENT_THREAD_H_

#include <pthread.h>
#include <stdint.h>

#include <string>

namespace CurrentThread
{
// extern __thread int t_cachedTid;
extern __thread const char * t_threadName;

// pid_t getTid();
void cacheTid();
int tid();
const char * name();

bool isMainThread();
void sleepUsec(int64_t usec);
std::string stackTrace(bool demangle);
} // namespace CurrentThread

#endif