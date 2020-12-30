#ifndef LOG_STREAM_H_
#define LOG_STREAM_H_

#include <assert.h>
#include <string.h>

#include <functional>

#include "NonCopyable.h"
#include "StringPiece.h"
#include "Types.h"

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer : private NonCopyable
{
public:
    typedef std::function<void()> CookieFuncType;

public:
    FixedBuffer()
        : m_pCur(m_data)
        , m_cookieFunc(NULL)
    {
        setCookie(cookieStart);
    }

    ~FixedBuffer()
    {
        setCookie(cookieEnd);
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

    void add(size_t n)
    {
        m_pCur = m_pCur + n;
    }

    void reset()
    {
        m_pCur = m_data;
    }

    void bzero()
    {
        memZero(m_data, sizeof(m_data));
    }

    const char * debugString()
    {
        *m_pCur = '\0';
        return m_data;
    }

    void setCookie(const CookieFuncType & cookieFunc)
    {
        m_cookieFunc = cookieFunc;
    }

    std::string toString() const
    {
        return std::string(m_data, length());
    }

    StringPiece toStringPiece() const
    {
        return StringPiece(m_data, length());
    }

    void append(const char * buf, size_t len)
    {
        if (implicit_cast<size_t>(avail()) > len)
        {
            memcpy(m_pCur, buf, len);
            add(len);
        }
    }

private:
    const char * end() const
    {
        return m_data + sizeof(m_data);
    }

private:
    static void cookieStart()
    {
    }
    static void cookieEnd()
    {
    }

private:
    char m_data[SIZE];
    char * m_pCur;
    CookieFuncType m_cookieFunc;
};

class LogStream : private NonCopyable
{
public:
    typedef FixedBuffer<kSmallBuffer> BufferType;
public:
    LogStream & log(bool v);
    LogStream & log(short v);
    LogStream & log(int v);
    LogStream & log(long v);
    LogStream & log(long long v);
    LogStream & log(unsigned short v);
    LogStream & log(unsigned int v);
    LogStream & log(unsigned long v);
    LogStream & log(unsigned long long v);
    LogStream & log(const void * p);
    LogStream & log(float v);
    LogStream & log(double v);
    LogStream & log(char v);
    LogStream & log(const char * str);
    LogStream & log(const unsigned char * str);
    LogStream & log(const std::string & str);
    LogStream & log(const StringPiece & str);
    LogStream & log(const BufferType & v);

    void append(const char * data, int len)
    {
        m_buffer.append(data, len);
    }
    const BufferType & buffer() const
    {
        return m_buffer;
    }

    void resetBuffer()
    {
        m_buffer.reset();
    }
private:
    template<typename T>
    void formatInteger(T v);

private:
    static const int kMaxNumbericSize = 32;
    BufferType m_buffer;
};

#endif