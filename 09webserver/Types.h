#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <string.h>
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

using std::string;

inline void memZero(void * p, size_t n)
{
    memset(p, 0, n);
}

template <typename To, typename From>
inline To implicit_cast(const From & f)
{
    return f;
}

template <typename To, typename From>
inline To down_cast(From * f)
{
#ifndef NDEBUG
    assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
    return static_cast<To>(f);
}

#endif