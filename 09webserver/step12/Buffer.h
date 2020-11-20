#ifndef BUFFER_H_
#define BUFFER_H_

#include <algorithm>
#include <vector>

#include <assert.h>
#include <string.h>

#include "Endian.h"

#include "Copyable.h"
#include "StringPiece.h"
#include "Types.h"

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode
class Buffer : public Copyable
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : m_vecBuffer(kCheapPrepend + initialSize)
        , m_readerIndex(kCheapPrepend)
        , m_writerIndex(kCheapPrepend)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initialSize);
        assert(prependableBytes() == kCheapPrepend);
    }

    void swap(Buffer & rhs)
    {
        m_vecBuffer.swap(rhs.m_vecBuffer);
        std::swap(m_readerIndex, rhs.m_readerIndex);
        std::swap(m_writerIndex, rhs.m_writerIndex);
    }

    size_t readableBytes() const
    {
        return m_writerIndex - m_readerIndex;
    }

    size_t writableBytes() const
    {
        return m_vecBuffer.size() - m_writerIndex;
    }

    size_t prependableBytes() const
    {
        return m_readerIndex;
    }

    const char * peek() const
    {
        return begin() + m_readerIndex;
    }

    const char * findCRLF() const
    {
        /*
        template< class ForwardIt1, class ForwardIt2 >
        ForwardIt1 search( ForwardIt1 first, ForwardIt1 last,
                        ForwardIt2 s_first, ForwardIt2 s_last );
        */
        const char * crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    const char * findCRLF(const char * start) const
    {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const char * crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    const char * findEOL() const
    {
        /* const void * memchr ( const void * ptr, int value, size_t num ); */
        const void * eol = memchr(peek(), '\n', readableBytes());
        return static_cast<const char *>(eol);
    }

    const char * findEOL(const char * start) const
    {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const void * eol = memchr(start, '\n', beginWrite() - start);
        return static_cast<const char *>(eol);
    }

    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        if (len < readableBytes())
        {
            m_readerIndex += len;
        }
        else
        {
            retrieveAll();
        }
    }

    void retrieveUntil(const char * end)
    {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
    }

    void retrieveInt64()
    {
        retrieve(sizeof(int64_t));
    }

    void retrieveInt32()
    {
        retrieve(sizeof(int32_t));
    }

    void retrieveInt16()
    {
        retrieve(sizeof(int16_t));
    }

    void retrieveInt8()
    {
        retrieve(sizeof(int8_t));
    }

    void retrieveAll()
    {
        m_readerIndex = kCheapPrepend;
        m_writerIndex = kCheapPrepend;
    }

    string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    string retrieveAsString(size_t len)
    {
        assert(len <= readableBytes());
        string result(peek(), len);
        retrieve(len);
        return result;
    }

    StringPiece toStringPiece() const
    {
        return StringPiece(peek(), static_cast<int>(readableBytes()));
    }

    // write range
    char * beginWrite()
    {
        return begin() + m_writerIndex;
    }

    const char * beginWrite() const
    {
        return begin() + m_writerIndex;
    }

    void hasWritten(size_t len)
    {
        assert(len <= writableBytes());
        m_writerIndex += len;
    }

    void unWrite(size_t len)
    {
        assert(len <= readableBytes());
        m_writerIndex -= len;
    }

    void append(const StringPiece & str)
    {
        append(str.data(), str.size());
    }

    void append(const char * data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    void append(const void * data, size_t len)
    {
        append(static_cast<const char *>(data), len);
    }

    void ensureWritableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }

    void appendInt64(int64_t x)
    {
        int64_t be64 = sockets::hostToNetwork64(x);
        append(&be64, sizeof(be64));
    }

    void appendInt32(int32_t x)
    {
        int32_t be32 = sockets::hostToNetwork32(x);
        append(&be32, sizeof(be32));
    }

    void appendInt16(int16_t x)
    {
        int16_t be16 = sockets::hostToNetwork16(x);
        append(&be16, sizeof(be16));
    }

    void appendInt8(int8_t x)
    {
        append(&x, sizeof(x));
    }

    int64_t readInt64()
    {
        int64_t result = peekInt64();
        retrieveInt64();
        return result;
    }

    int32_t readInt32()
    {
        int32_t result = peekInt32();
        retrieveInt32();
        return result;
    }

    int16_t readInt16()
    {
        int16_t result = peekInt16();
        retrieveInt16();
        return result;
    }

    int8_t readInt8()
    {
        int8_t result = peekInt8();
        retrieveInt8();
        return result;
    }

    int64_t peekInt64() const
    {
        assert(readableBytes() >= sizeof(int64_t));
        int64_t be64 = 0;
        ::memcpy(&be64, peek(), sizeof(be64));
        return sockets::networkToHost64(be64);
    }

    int32_t peekInt32() const
    {
        assert(readableBytes() >= sizeof(int32_t));
        int32_t be32 = 0;
        ::memcpy(&be32, peek(), sizeof(be32));
        return sockets::networkToHost32(be32);
    }

    int32_t peekInt16() const
    {
        assert(readableBytes() >= sizeof(int16_t));
        int16_t be16 = 0;
        ::memcpy(&be16, peek(), sizeof(be16));
        return sockets::networkToHost16(be16);
    }

    int8_t peekInt8() const
    {
        assert(readableBytes() >= sizeof(int8_t));
        int8_t x = *peek();
        return x;
    }

    void prependInt64(int64_t x)
    {
        int64_t be64 = sockets::hostToNetwork64(x);
        prepend(&be64, sizeof(be64));
    }

    void prependInt32(int32_t x)
    {
        int32_t be32 = sockets::hostToNetwork32(x);
        prepend(&be32, sizeof be32);
    }

    void prependInt16(int16_t x)
    {
        int16_t be16 = sockets::hostToNetwork16(x);
        prepend(&be16, sizeof be16);
    }

    void prependInt8(int8_t x)
    {
        prepend(&x, sizeof x);
    }

    void prepend(const void * /*restrict*/ data, size_t len)
    {
        assert(len <= prependableBytes());
        m_readerIndex -= len;
        const char * d = static_cast<const char *>(data);
        std::copy(d, d + len, begin() + m_readerIndex);
    }

    void shrink(size_t reserve)
    {
        // FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
        Buffer other;
        other.ensureWritableBytes(readableBytes() + reserve);
        other.append(toStringPiece());
        swap(other);
    }

    size_t internalCapacity() const
    {
        return m_vecBuffer.capacity();
    }

    ssize_t readFd(int fd, int * savedErrno);

private:
    char * begin()
    {
        return m_vecBuffer.data();
    }

    const char * begin() const
    {
        return m_vecBuffer.data();
    }

    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            // FIXME: move readable data
            m_vecBuffer.resize(m_writerIndex + len);
        }
        else
        {
            // move readable data to the front, make space inside buffer
            assert(kCheapPrepend < m_readerIndex);
            size_t readable = readableBytes();
            std::copy(begin() + m_readerIndex,
                      begin() + m_writerIndex,
                      begin() + kCheapPrepend);
            m_readerIndex = kCheapPrepend;
            m_writerIndex = m_readerIndex + readable;
            assert(readable == readableBytes());
        }
    }

private:
    std::vector<char> m_vecBuffer;
    size_t m_readerIndex;
    size_t m_writerIndex;

    static const char kCRLF[];
};

#endif