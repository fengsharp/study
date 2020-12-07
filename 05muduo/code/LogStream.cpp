#include "LogStream.h"

#include <algorithm>
#include <assert.h>
#include <limits>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <type_traits>

const char digits[] = "9876543210123456789";
const char * zero = digits + 9;
static_assert(sizeof(digits) == 20, "wront number of digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex) == 17, "wrong number of digitsHex");

template <typename T>
size_t convert(char buf[], T value)
{
    T i = value;
    char * p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p = zero[lsd];
        ++p;
    } while (i != 0);

    if (vlaue < 0)
    {
        *p = '-';
        ++p;
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

// typedef unsigned long int	uintptr_t;
size_t convertHex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char * p = buf;

    do
    {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p = digitsHex[lsd];
        ++p;
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargebuffer>;

///------------------------------

void LogStream::staticCheck()
{
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10, "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10, "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10, "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10, "kMaxNumericSize is large enough");
}

template <typename T>
void LogStream::formatInteger(T v)
{
    if (m_buffer.avail() >= kMaxNumericSize)
    {
        size_t len = convert(m_buffer.current(), v);
        m_buffer.add(len);
    }
}

LogStream & LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream & LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream & LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::operator<<(const void * p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (m_buffer.avail() >= kMaxNumericSize)
    {
        char * buf = m_buffer.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = convert(buf + 2, v);
        m_buffer.add(len + 2);
    }
    return *this;
}

LogStream & LogStream::operator<<(double v)
{
    if (m_buffer.avail() >= kMaxNumericSize)
    {
        // %g是C语言printf()函数的bai一个输出格式类型
        // 它表示以%f%e中较短的输出宽度输出单、双精度实数，在指数小于-4或者大于等于精度时使用%e格式
        int len = snprintf(m_buffer.current(), kMaxNumericSize, "%.12g", v);
        m_buffer.add(len);
    }
    return *this;
}

template <typename T>
Fmt::Fmt(const char * fmt, T val)
{
    static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type");

    m_length = snprintf(m_buf, sizeof(m_buf), fmt, val);
    assert(static_cast<size_t>(m_length) < sizeof(m_buf));
}

template Fmt::Fmt(const char * fmt, char);

template Fmt::Fmt(const char * fmt, short);
template Fmt::Fmt(const char * fmt, unsigned short);
template Fmt::Fmt(const char * fmt, int);
template Fmt::Fmt(const char * fmt, unsigned int);
template Fmt::Fmt(const char * fmt, long);
template Fmt::Fmt(const char * fmt, unsigned long);
template Fmt::Fmt(const char * fmt, long long);
template Fmt::Fmt(const char * fmt, unsigned long long);

template Fmt::Fmt(const char * fmt, float);
template Fmt::Fmt(const char * fmt, double);