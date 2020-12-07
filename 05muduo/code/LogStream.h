#ifndef LOG_STREAM_H_
#define LOG_STREAM_H_

#include <assert.h>
#include <string.h>

#include "Noncopyable.h"
#include "StringPiece.h"
#include "Types.h"

constexpr int kSmallBuffer = 4000; // 4k
constexpr int kLargebuffer = 4000 * 1024; // 4M

template <int SIZE>
class FixedBuffer : private NonCopyable
{
public:
    FixedBuffer()
        : m_pCur(m_data)
    {
    }

    ~FixedBuffer()
    {
    }

    void append(const char * buf, size_t len)
    {
        if (implicit_cast<size_t>(avail()) > len)
        {
            memcpy(m_pCur, buf, len);
            m_pCur += len;
        }
    }

    const char * data() const
    {
        return m_data;
    }

    int length() const
    {
        return static_cast<int>(m_pCur - m_data);
    }

    char * current()
    {
        return m_pCur;
    }

    int avail() const
    {
        return static_cast<int>(end() - m_pCur);
    }

    void add(size_t len)
    {
        m_pCur += len;
    }

    void reset()
    {
        m_pCur = m_data;
    }

    void bzero()
    {
        memZero(m_data, sizeof(m_data));
    }

    std::string toString() const
    {
        return std::string(m_data, length());
    }

    StringPiece toStringPiece() const
    {
        return StringPiece(m_data, length());
    }

private:
    const char * end()
    {
        return m_data + sizeof(m_data);
    }

private:
    char m_data[SIZE];
    char * m_pCur;
};

class LogStream : private NonCopyable
{
public:
    LogStream & operator<<(bool v)
    {
        m_buffer.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream & operator<<(short v);
    LogStream & operator<<(unsigned short v);
    LogStream & operator<<(int v);
    LogStream & operator<<(unsigned int v);
    LogStream & operator<<(long v);
    LogStream & operator<<(unsigned long v);
    LogStream & operator<<(long long v);
    LogStream & operator<<(unsigned long long v);

    LogStream & operator<<(const void * p);

    LogStream & operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }

    LogStream & operator<<(double v);

    LogStream & operator<<(char v)
    {
        m_buffer.append(&v, 1);
        return *this;
    }

    LogStream & operator<<(const char * str)
    {
        if (str)
        {
            m_buffer.append(str, strlen(str));
        }
        else
        {
            m_buffer.append("(null)", strlen("(null)"));
        }
        return *this;
    }

    LogStream & operator<<(unsigned char * str)
    {
        return operator<<(reinterpret_cast<const char *>(str));
    }
    LogStream & operator<<(const std::string & v)
    {
        m_buffer.append(v.data(), v.size());
        return *this;
    }
    LogStream & operator<<(const StringPiece & v)
    {
        m_buffer.append(v.data(), v.size());
        return *this;
    }
    LogStream & operator<<(const FixedBuffer<kSmallBuffer> & v)
    {
        *this << v.toStringPiece();
        return *this;
    }

    void append(const char * data, int len)
    {
        m_buffer.append(data, len);
    }

    const FixedBuffer<kSmallBuffer> & buffer() const
    {
        return m_buffer;
    }

    void resetBuffer()
    {
        m_buffer.reset();
    }

private:
    void staticCheck();
    template <typename T>
    void formatInteger(T);

private:
    FixedBuffer<kSmallBuffer> m_buffer;
    static const int kMaxNumericSize = 32;
};

class Fmt : private NonCopyable
{
public:
    template<typename T>
    Fmt(const char * fmt, T val);

    const char * data() const
    {
        return m_buf;
    }

    int length() const
    {
        return m_length;
    }
private:
    char m_buf[32];
    int m_length;
};

inline LogStream & operator<<(LogStream & s, const Fmt & fmt)
{
    s.append(fmt.data(), fmt.length());
    return s;
}

#endif