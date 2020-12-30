#include "LogStream.h"

#include <algorithm>
#include <assert.h>
#include <limits>
#include <stdint.h>
#include <stdio.h>
#include <type_traits>

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

const char digits[] = "9876543210123456789";
const char * zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong number of digits");

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

    if (value < 0)
    {
        *p = '-';
        ++p;
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

/*
intptr_t是为了跨平台，其长度总是所在平台的位数，所以用来存放地址。
uintptr_t 是intptr_t 的无符号版本
*/
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

LogStream & LogStream::log(bool v)
{
    m_buffer.append(v ? "1" : "0", 1);
    return *this;
}

template <typename T>
void LogStream::formatInteger(T v)
{
    if (m_buffer.avail() >= kMaxNumbericSize)
    {
        size_t len = convert(m_buffer.current(), v);
        m_buffer.add(len);
    }
}

LogStream & LogStream::log(short v)
{
    return log(static_cast<int>(v));
}

LogStream & LogStream::log(int v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::log(long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::log(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::log(unsigned short v)
{
    return log(static_cast<unsigned int>(v));
}

LogStream & LogStream::log(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::log(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::log(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream & LogStream::log(const void * p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (m_buffer.avail() >= kMaxNumbericSize)
    {
        char * buf = m_buffer.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t n = convertHex(buf + 2, v);
        m_buffer.add(n);
    }
    return *this;
}

LogStream & LogStream::log(float v)
{
    return log(static_cast<double>(v));
}

LogStream & LogStream::log(double v)
{
    if (m_buffer.avail() >= kMaxNumbericSize)
    {
        // %g表示以%f%e中较短的输出宽度输出单、双精度实数，在指数小于-4或者大于等于精度时使用%e格式
        int len = snprintf(m_buffer.current(), kMaxNumbericSize, "%.12g", v);
        m_buffer.add(len);
    }
    return *this;
}

LogStream & LogStream::log(char v)
{
    m_buffer.append(&v, 1);
    return *this;
}

LogStream & LogStream::log(const char * str)
{
    if (str)
    {
        m_buffer.append(str, strlen(str));
    }
    else
    {
        m_buffer.append("(null)", 6);
    }
    return *this;
}

LogStream & LogStream::log(const unsigned char * str)
{
    return log(reinterpret_cast<const char *>(str));
}

LogStream & LogStream::log(const std::string & str)
{
    m_buffer.append(str.data(), str.size());
    return *this;
}

LogStream & LogStream::log(const StringPiece & str)
{
    m_buffer.append(str.data(), str.size());
    return *this;
}

LogStream & LogStream::log(const BufferType & v)
{
    return log(v.toStringPiece());
}
