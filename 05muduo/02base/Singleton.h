#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <assert.h>
#include <stdlib.h>

#include <pthread.h>

#include "NonCopyable.h"

template <typename T>
class Singleton : private NonCopyable
{
public:
    Singleton() = delete;
    ~Singleton() = delete;

    static T * instance()
    {
        pthread_once(&m_onceContrl, &Singleton::onceFunc);
        assert(m_pValue != NULL);
        return m_pValue;
    }

private:
    static void onceFunc()
    {
        m_pValue = new T();
        ::atexit(&Singleton::exitFunc);
    }

    static void exitFunc()
    {
        if (m_pValue)
        {
            delete m_pValue;
            m_pValue = NULL;
        }
    }

private:
    static T * m_pValue;
    static pthread_once_t m_onceContrl;
};

template <typename T>
T * Singleton<T>::m_pValue = NULL;

template <typename T>
pthread_once_t Singleton<T>::m_onceContrl = PTHREAD_ONCE_INIT;

#endif