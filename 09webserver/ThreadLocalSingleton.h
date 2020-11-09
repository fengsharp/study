#ifndef THREAD_LOCAL_SINGLETON_H_
#define THREAD_LOCAL_SINGLETON_H_

#include <assert.h>
#include <pthread.h>

#include "NoneCopyable.h"

template <typename T>
class ThreadLocalSingleton : private NoneCopyable
{
public:
    ThreadLocalSingleton() = delete;
    ~ThreadLocalSingleton() = delete;

    static T & instance()
    {
        if (m_t_value == NULL)
        {
            m_t_value = new T();
            m_deleter.set(m_t_value);
        }
        return *m_t_value;
    }

    static T * pointer()
    {
        return m_t_value;
    }

private:
    static void destructor(void * pObj)
    {
        assert(pObj == m_t_value);
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void)dummy;
        delete m_t_value;
        m_t_value = 0;
    }

private:
    class Deleter
    {
    public:
        Deleter()
        {
            pthread_key_create(&m_t_pkey, &ThreadLocalSingleton::destructor);
        }
        ~Deleter()
        {
            pthread_key_delete(m_t_pkey);
        }

        void set(T * newObj)
        {
            assert(pthread_getspecific(m_t_pkey) == NULL);
            pthread_setspecific(m_t_pkey, newObj);
        }

    private:
        pthread_key_t m_t_pkey;
    };

private:
    static __thread T * m_t_value;
    static Deleter m_deleter;
};

template <typename T>
__thread T * ThreadLocalSingleton<T>::m_t_value = 0;

template<typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::m_deleter;

#endif