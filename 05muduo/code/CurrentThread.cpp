#include "CurrentThread.h"

#include <time.h>

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

#include "Timestamp.h"

namespace CurrentThread
{
__thread int t_cachedTid;
__thread char t_tidString[32];
__thread int t_tidStringLength;
__thread const char * t_threadName;

pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
    }
}

bool isMainThread()
{
    return t_cachedTid == gettid();
}

void sleepUsec(int64_t usec)
{
    // int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
    struct timespec ts = { 0, 0 };
    ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicrosecondsPerSeconds);
    ts.tv_nsec = static_cast<time_t>(usec % Timestamp::kMicrosecondsPerSeconds * 1000);
    ::nanosleep(&ts, NULL);
}

std::string stackTrace(bool demangle)
{
    std::string stack;
    const int max_frames = 200;
    void * frame[max_frames];
    int nptrs = ::backtrace(frame, max_frames);
    char ** strings = ::backtrace_symbols(frame, nptrs);
    if (strings)
    {
        size_t len = 256;
        char * demangled = demangle ? static_cast<char *>(::malloc(len)) : nullptr;
        for (int i = 1; i < nptrs; ++i) // skipping the 0-th, which is this function
        {
            if (demangle)
            {
                // https://panthema.net/2008/0901-stacktrace-demangled/
                // bin/exception_test(_ZN3Bar4testEv+0x79) [0x401909]
                char * left_par = nullptr;
                char * plus = nullptr;
                for (char * p = strings[i]; *p; ++p)
                {
                    if (*p == '(')
                        left_par = p;
                    else if (*p == '+')
                        plus = p;
                }

                if (left_par && plus)
                {
                    *plus = '\0';
                    int status = 0;
                    char * ret = abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
                    *plus = '+';
                    if (status == 0)
                    {
                        demangled = ret; // ret could be realloc()
                        stack.append(strings[i], left_par + 1);
                        stack.append(demangled);
                        stack.append(plus);
                        stack.push_back('\n');
                        continue;
                    }
                }
            }
            // Fallback to mangled names
            stack.append(strings[i]);
            stack.push_back('\n');
        }
        free(demangled);
        free(strings);
    }
    return stack;
}

} // namespace CurrentThread