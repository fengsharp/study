#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <sys/types.h>

#include "NonCopyable.h"
#include "StringPiece.h"

namespace FileUtil
{
class ReadSmallFile : private NonCopyable
{
public:
    static const int kBufferSize = 64 * 1024;

    ReadSmallFile(StringArg filename);
    ~ReadSmallFile();

    template <typename String>
    int readToString(int maxSize, String * content, int64_t * fileSize, int64_t * modifyTime, int64_t * createTime);
    int readToBuffer(int * size);
    const char * buffer() const
    {
        return m_buf;
    }

private:
    int m_fd;
    int m_err;
    char m_buf[kBufferSize];
};

template <typename String>
int readFile(StringArg filename, int maxSize, String * content, int64_t * fileSize = NULL, int64_t * modifyTime = NULL, int64_t * createTime = NULL)
{
    ReadSmallFile file(filename);
    return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

class AppendFile : private NonCopyable
{
public:
    explicit AppendFile(StringArg filename);
    ~AppendFile();
    void append(const char * logline, size_t len);
    void flush();
    off_t writtenBytes() const
    {
        return m_writtenBytes;
    }

private:
    size_t write(const char * logline, size_t len);

private:
    static const int kBufferSize = 64 * 1024;

    FILE * m_fp;
    char m_buffer[kBufferSize];
    off_t m_writtenBytes;
};
} // namespace FileUtil

#endif