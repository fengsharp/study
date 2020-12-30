#include "CurrentThread.h"

#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include <string>

#include "Timestamp.h"

namespace CurrentThread
{
__thread int t_cachedTid = 0;
__thread const char * t_threadName = "Unknown";

class InitMainThread
{
public:
    InitMainThread()
    {
        cacheTid();
        t_threadName = "main";
    }
};
InitMainThread init;

pid_t getTid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = getTid();
    }
}

int tid()
{
    /*
    __builtin_expect((x),1)表示 x 的值为真的可能性更大；
    __builtin_expect((x),0)表示 x 的值为假的可能性更大。
    */
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
        cacheTid();
    }
    return t_cachedTid;
}

const char * name()
{
    return t_threadName;
}

bool isMainThread()
{
    return tid() == ::getpid();
}

void sleepUsec(int64_t usec)
{
    // int nanosleep(const struct timespec *req, struct timespec *rem);
    struct timespec tmspec = { 0, 0 };
    tmspec.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSeconds);
    tmspec.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSeconds * 1000);
    ::nanosleep(&tmspec, NULL);
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