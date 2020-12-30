#ifndef LOGGING_H_
#define LOGGING_H_

#include <functional>

#include "LogStream.h"
#include "Timestamp.h"

class SourceFile
{
public:
    template <int N>
    SourceFile(const char (&arr)[N])
        : m_data(arr)
        , m_size(N - 1)
    {
        const char * slash = strrchr(m_data, '/');
        if (slash)
        {
            m_data = slash + 1;
            m_size -= static_cast<int>(m_data - arr);
        }
    }

    explicit SourceFile(const char * fileName)
        : m_data(fileName)
    {
        const char * slash = strrchr(m_data, '/');
        if (slash)
        {
            m_data = slash + 1;
            m_size = static_cast<int>(strlen(m_data));
        }
    }

    const char * data() const
    {
        return m_data;
    }

    int size() const
    {
        return m_size;
    }

private:
    const char * m_data;
    int m_size;
};

enum LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
};

class Logger
{
public:
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char * func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream & stream()
    {
        return m_impl.stream;
    }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef std::function<void(const char * msg, int len)> OutputFuncType;
    typedef std::function<void()> FlushFuncType;
    static void setOutput(OutputFuncType);
    static void setFlush(FlushFuncType);

private:
    class Impl
    {
    public:
        Impl(LogLevel level, int old_errno, const SourceFile & file, int line);
        void formatTime();
        void finish();

        LogStream stream;
        LogLevel level;

        Timestamp time;
        SourceFile baseName;
        int line;
    };
    Impl m_impl;
};

#define LOG_TRACE                            \
    if (Logger::logLevel() <= Logger::TRACE) \
    Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG                            \
    if (Logger::logLevel() <= Logger::DEBUG) \
    Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO                            \
    if (Logger::logLevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, true).stream()

const char * strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
    ::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T * CheckNotNull(SourceFile file, int line, const char * names, T * ptr)
{
    if (ptr == NULL)
    {
        Logger(file, line, LogLevel::FATAL).stream().log(names);
    }
    return ptr;
}

#endif