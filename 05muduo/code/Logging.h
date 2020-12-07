#ifndef LOGGING_H_
#define LOGGING_H_

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

    explicit SourceFile(const char * filename)
        : m_data(filename)
    {
        const char * slash = strrchr(filename, '/');
        if (slash)
        {
            m_data = slash + 1;
        }
        m_size = static_cast<int>(strlen(m_data));
    }

    const char * m_data;
    int m_size;
};

class Logger
{
public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char * func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream & stream()
    {
        return m_impl.m_stream;
    }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char * msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);

private:
    class Impl
    {
    public:
        Impl(Logger::LogLevel level, int oldErrno, const SourceFile & file, int line);
        void formatTime();
        void finish();

    public:
        Timestamp m_time;
        LogStream m_stream;
        Logger::LogLevel m_level;
        int m_line;
        SourceFile m_basename;
    };

private:
    Impl m_impl;
};

extern Logger::LogLevel g_logLevel;
inline Logger::LogLevel Logger::logLevel()
{
    return g_logLevel;
}

#define LOG_TRACE                            \
    if (Logger::logLevel() <= Logger::TRACE) \
    Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()

#define LOG_DEBUG                            \
    if (Logger::logLevel() <= Logger::DEBUG) \
    Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()

#define LOG_INFO                            \
    if (Logger::logLevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__, Logger::INFO, __func__).stream()

#define LOG_WARN                            \
    if (Logger::logLevel() <= Logger::WARN) \
    Logger(__FILE__, __LINE__, Logger::WARN).stream()

#define LOG_ERROR                            \
    if (Logger::logLevel() <= Logger::ERROR) \
    Logger(__FILE__, __LINE__, Logger::ERROR).stream()

#define LOG_FATAL                            \
    if (Logger::logLevel() <= Logger::FATAL) \
    Logger(__FILE__, __LINE__, Logger::FATAL).stream()

#define LOG_SYSERR                           \
    if (Logger::logLevel() <= Logger::FATAL) \
    Logger(__FILE__, __LINE__, false).stream()

#define LOG_SYSFATAL                         \
    if (Logger::logLevel() <= Logger::FATAL) \
    Logger(__FILE__, __LINE__, true).stream()

const char * strerror_tl(int saveErrno);

#endif