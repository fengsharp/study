#ifndef MUDUO_BASE_FILEUTIL_H
#define MUDUO_BASE_FILEUTIL_H

#include <stdio.h>

#include "NoneCopyable.h"
#include "StringPiece.h"

namespace FileUtil
{
// read small file < 64KB
class ReadSmallFile : private NoneCopyable
{
public:
    explicit ReadSmallFile(StringArg filename);
    ~ReadSmallFile();

    // return errno
    template <typename String>
    int readToString(int maxSize,
                     String * content,
                     int64_t * fileSize,
                     int64_t * modifyTime,
                     int64_t * createTime);

    /// Read at maxium kBufferSize into buf_
    // return errno
    int readToBuffer(int * size);

    const char * buffer() const
    {
        return buf_;
    }

    static const int kBufferSize = 64 * 1024;

private:
    int fd_;
    int err_;
    char buf_[kBufferSize];
};

// read the file content, returns errno if error happens.
template <typename String>
int readFile(StringArg filename,
             int maxSize,
             String * content,
             int64_t * fileSize = NULL,
             int64_t * modifyTime = NULL,
             int64_t * createTime = NULL)
{
    ReadSmallFile file(filename);
    return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

// not thread safe
class AppendFile : private NoneCopyable
{
public:
    explicit AppendFile(StringArg filename);
    ~AppendFile();

    void append(const char * logline, size_t len);
    void flush();
    off_t writtenBytes() const
    {
        return writtenBytes_;
    }

private:
    size_t write(const char * logline, size_t len);

private:
    FILE * fp_;
    char buffer_[64 * 1024];
    off_t writtenBytes_;
};
} // namespace FileUtil

#endif