#ifndef TYPES_H_
#define TYPES_H_

/*
数值类型
    各种宽度整形
    浮点型
字符串
bool类型
*/
#include <stdint.h>
#include <string.h>
#ifndef NDEBUG
#include <assert.h>
#endif
#include <string>

inline void memZero(void * p, size_t n)
{
    memset(p, 0, n);
}

/// 隐式转换，如子类多路继承时，转为父类，替换掉static_cast
template <typename From, typename To>
inline To implicit_cast(const From & f)
{
    return f;
}

/// 父类指针转为子类指针，使用static_cast，替换掉dynamic_cast
template <typename From, typename To>
inline To down_cast(From * f)
{
#ifndef NDEBUG
    assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
    return static_cast<To>(f);
}

#endif