#ifndef MUDUO_BASE_THREADLOCAL_H
#define MUDUO_BASE_THREADLOCAL_H

#include "MutexLock.h"
#include "noncopyable.h"

#include <pthread.h>

template<typename T>
class ThreadLocal : noncopyable
{
public:
    ThreadLocal()
    {
        MCHECK(pthread_key_create(&m_pthreadKey, &ThreadLocal::destructor));
    }

    ~ThreadLocal()
    {
        MCHECK(pthread_key_delete(m_pthreadKey));
    }

    T& value()
    {
        T* perThreadValue = static_cast<T*>(pthread_getspecific(m_pthreadKey));
        if (!perThreadValue)
        {
            T* newObj = new T();
            MCHECK(pthread_setspecific(m_pthreadKey, newObj));
            perThreadValue = newObj;
        }
        return *perThreadValue;
    }

private:
    static void destructor(void *x)
    {
        T* obj = static_cast<T*>(x);
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy; (void) dummy;
        delete obj;
    }

private:
    pthread_key_t m_pthreadKey;
};


#endif