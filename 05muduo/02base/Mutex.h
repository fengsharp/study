#ifndef MUTEX_H_
#define MUTEX_H_

#include <assert.h>
#include <pthread.h>

#include "CurrentThread.h"
#include "NonCopyable.h"

// Thread safety annotations {
// https://clang.llvm.org/docs/ThreadSafetyAnalysis.html

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x) // no-op
#endif

#define CAPABILITY(x) \
    THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define SCOPED_CAPABILITY \
    THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define GUARDED_BY(x) \
    THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

#define PT_GUARDED_BY(x) \
    THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(x))

#define ACQUIRED_BEFORE(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define ACQUIRED_AFTER(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#define REQUIRES(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define REQUIRES_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#define ACQUIRE(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define ACQUIRE_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define RELEASE(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define RELEASE_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define TRY_ACQUIRE(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define TRY_ACQUIRE_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#define EXCLUDES(...) \
    THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define ASSERT_CAPABILITY(x) \
    THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#define ASSERT_SHARED_CAPABILITY(x) \
    THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

#define RETURN_CAPABILITY(x) \
    THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))

#define NO_THREAD_SAFETY_ANALYSIS \
    THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)

// End of thread safety annotations }

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail(int errnum,
                                 const char * file,
                                 unsigned int line,
                                 const char * function) noexcept __attribute__((__noreturn__));
__END_DECLS
#endif

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__); })

#else // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum; })

#endif // CHECK_PTHREAD_RETURN_VALUE

/*
CAPABILITY(<string>)
CAPABILITY是一种类的属性，它意味着该类的对象可以被当做监护权使用。string参数使用错误信息指定了监护权的类型，例如“mutex"。

SCOPED_CAPABILITY
SCOPED_CAPABILITY是一种类的属性，这种类实现了RAII风格的锁，监护权在构造函数中获取，在析构函数中释放。这种类需要被特别指出，因为构造和析构函数指定的监护权的名称是不一样的

ASSERT_CAPABILITY(...)和ASSERT_SHARED_CAPABILITY(...)
这是一种函数或方法的属性，它表明该函数将在运行时进行一个安全检查，判断调用线程是否拥有监护权。如果调用线程没有监护权，该函数将会返回空表明调用失败。
*/
class CAPABILITY("mutex") MutexLock : private NonCopyable
{
public:
    MutexLock()
        : m_holder(0)
    {
        pthread_mutex_init(&m_mutex, NULL);
    }

    ~MutexLock()
    {
        assert(m_holder == 0);
        pthread_mutex_destroy(&m_mutex);
    }

    bool isLockedByThisthread() const
    {
        return m_holder == CurrentThread::tid();
    }

    void assertLocked() const ASSERT_CAPABILITY(this)
    {
        assert(isLockedByThisthread());
    }

    void lock() ACQUIRE()
    {
        pthread_mutex_lock(&m_mutex);
        assignHolder();
    }

    void unlock() RELEASE()
    {
        unassignHolder();
        pthread_mutex_unlock(&m_mutex);
    }

private:
    friend class Condition;

    class UnassignGuard : private NonCopyable
    {
    public:
        explicit UnassignGuard(MutexLock & owner)
            : m_owner(owner)
        {
            m_owner.unassignHolder();
        }

        ~UnassignGuard()
        {
            m_owner.assignHolder();
        }

    private:
        MutexLock & m_owner;
    };

private:
    void assignHolder()
    {
        m_holder = CurrentThread::tid();
    }

    void unassignHolder()
    {
        m_holder = 0;
    }

    pthread_mutex_t * getPthreadMutex()
    {
        return &m_mutex;
    }
private:
    pthread_mutex_t m_mutex;
    pid_t m_holder;
};

class SCOPED_CAPABILITY MutexLockGuard : private NonCopyable
{
public:
    explicit MutexLockGuard(MutexLock & mutex) ACQUIRE(mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~MutexLockGuard() RELEASE()
    {
        m_mutex.unlock();
    }

private:
    MutexLock & m_mutex;
};

#define MutexLockGuard(x) error "Missing guard object name"

#endif