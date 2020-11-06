#ifndef THREAD_LOCAL_H_
#define THREAD_LOCAL_H_

#include <pthread.h>

#include "Mutex.h"
#include "NoneCopyable.h"

template <typename T>
class ThreadLocal : private NoneCopyable
{
public:
    ThreadLocal()
    {
        MCHECK(pthread_key_create(&m_pkey, &ThreadLocal::destructor));
    }

    ~ThreadLocal()
    {
        MCHECK(pthread_key_delete(m_pkey));
    }

    T & value()
    {
        T * perThreadValue = static_cast<T *>(pthread_getspecific(m_pkey));
        if (perThreadValue == NULL)
        {
            T * newObj = new T();
            MCHECK(pthread_setspecific(m_pkey, newObj));
            perThreadValue = newObj;
        }
        return *perThreadValue;
    }

private:
    static void destructor(void * x)
    {
        T * obj = static_cast<T *>(x);
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void)dummy;
        delete obj;
        obj = NULL;
    }

private:
    pthread_key_t m_pkey;
};

#endif