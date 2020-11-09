#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "NoneCopyable.h"

template <typename T>
struct HasNoDestroy
{
    template <typename C>
    static char test(decltype(&C::no_destroy));
    template <typename C>
    static int32_t test(...);

    // 判断如果是类的话，是否有no_destroy方法
    const static bool value = sizeof(test<T>(0)) == 1;
};

template <typename T>
class Singleton : private NoneCopyable
{
public:
    Singleton() = delete;
    ~Singleton() = delete;

    static T & instance()
    {
        pthread_once(&m_ponce, &Singleton::init);
        assert(m_pValue != NULL);
        return *m_pValue;
    }

private:
    static void init()
    {
        m_pValue = new T();
        // 当参数是类且没有"no_destroy"方法才会注册atexit的destroy
        if (!HasNoDestroy<T>::value)
        {
            ::atexit(destroy);
        }
    }

    static void destroy()
    {
        // 用typedef定义了一个数组类型，数组的大小不能为-1
        // 利用这个方法，如果是不完全类型，编译阶段就会发现错误
        // 不完全类型”指在C++中有声明但又没有定义的类型
        // 用delete删除一个只有声明但无定义的类型的指针，是危险的。这通常导致无法调用析构函数
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void)dummy;

        delete m_pValue;
        m_pValue = NULL;
    }

private:
    static pthread_once_t m_ponce;
    static T * m_pValue;
};

template <typename T>
pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;

template <typename T>
T * Singleton<T>::m_pValue = NULL;

#endif