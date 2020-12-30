#include "Logging.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "CurrentThread.h"

__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_lastSecond;

const char * strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

static const char * LogLevelName[LogLevel::NUM_LOG_LEVELS] = {
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};

Logger::Impl::Impl(LogLevel level, int old_errno, const SourceFile & file, int line)
    : stream()
    , level(level)
    , time(Timestamp::now())
    , baseName(file)
    , line(line)
{
    formatTime();
    stream.log(CurrentThread::tid());
    stream.log(LogLevelName[level]);

    if (old_errno != 0)
    {
        stream.log(strerror_tl(old_errno));
        stream.log(" (errno=");
        stream.log(old_errno);
        stream.log(") ");
    }
}

void Logger::Impl::formatTime()
{
    stream.log(time.toFormattedString());
    stream.log(' ');
}

void Logger::Impl::finish()
{
    stream.log(" - ");
    stream.append(baseName.data(), baseName.size());
    stream.log(':');
    stream.log(line);
    stream.log('\n');
}

Logger::Logger(SourceFile file, int line)
    : m_impl(LogLevel::INFO, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level)
    : m_impl(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char * func)
    : m_impl(level, 0, file, line)
{
    m_impl.stream.log(func).log(' ');
}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : m_impl(toAbort ? LogLevel::FATAL : LogLevel::ERROR, errno, file, line)
{
}

void defaultOutput(const char * msg, int len)
{
    size_t n = fwrite(msg, 1, len, stdout);
    (void)n;
}

void defaultFlush()
{
    fflush(stdout);
}

LogLevel g_logLevel = LogLevel::INFO;
Logger::OutputFuncType g_output = defaultOutput;
Logger::FlushFuncType g_flush = defaultFlush;

Logger::~Logger()
{
    m_impl.finish();
    const LogStream::BufferType & buf = stream().buffer();
    g_output(buf.data(), buf.length());
    if (m_impl.level == LogLevel::FATAL)
    {
        g_flush();
        abort();
    }
}

LogLevel Logger::logLevel()
{
    return g_logLevel;
}

void Logger::setLogLevel(LogLevel level)
{
    g_logLevel = level;
}

void Logger::setOutput(OutputFuncType out)
{
    g_output = out;
}

void Logger::setFlush(FlushFuncType flush)
{
    g_flush = flush;
}
