#ifndef LOG_FILE_H_
#define LOG_FILE_H_

#include <memory>

#include "Types.h"
#include "Mutex.h"
#include "FileUtil.h"

class LogFile : private NonCopyable
{
public:
    LogFile(const std::string & basename, off_t rollSize, bool threadSafe=true, int flushInterval=3, int checkEveryN=1024);
    ~LogFile() = default;

    void append(const char * logline, int len);
    void flush();
    bool rollFile();

private:
    void append_unlocked(const char * logline, int len);
    static std::string getLogFileName(const std::string & basename, time_t * now);

private:
    static const int kRollPerSeconds = 60 * 60 * 24; 
    const std::string m_basename;
    const off_t m_rollSize;
    const int m_flushInterval;
    const int m_checkEveryN;
    int m_count;
    std::unique_ptr<MutexLock> m_mutex;
    time_t m_startOfPeriod;
    time_t m_lastRoll;
    time_t m_lastFlush;
    std::unique_ptr<FileUtil::AppendFile> m_file;
};

#endif