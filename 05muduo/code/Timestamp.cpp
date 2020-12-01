#include "Timestamp.h"

#include <sys/time.h>
#include <time.h>

Timestamp Timestamp::now()
{
    // int gettimeofday(struct timeval *tv, struct timezone *tz);
    struct timeval nowTime;
    gettimeofday(&nowTime, NULL);
    return Timestamp(nowTime.tv_sec * kMicrosecondsPerSeconds + nowTime.tv_usec);
}

std::string Timestamp::toString() const
{
    // seconds.microseconds
    char czBuf[32] = { 0 };
    int64_t seconds = m_microseconds / kMicrosecondsPerSeconds;
    int64_t microseconds = m_microseconds % kMicrosecondsPerSeconds;
    snprintf(czBuf, sizeof(czBuf) - 1, "%ld.%ld", seconds, microseconds);
    return czBuf;
}

std::string Timestamp::toFormattedString(bool isShowMicroseconds /* = true*/) const
{
    // 2020-12-01T14:33:00
    // struct tm *gmtime_r(const time_t *timep, struct tm *result);
    time_t seconds = static_cast<time_t>(m_microseconds / kMicrosecondsPerSeconds);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    char czBuf[64] = { 0 };
    if (isShowMicroseconds)
    {
        int microseconds = static_cast<int>(m_microseconds % kMicrosecondsPerSeconds);
        snprintf(czBuf, sizeof(czBuf) - 1, "%4d-%02d-%02dT%02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1900,
                 tm_time.tm_mon + 1,
                 tm_time.tm_mday,
                 tm_time.tm_hour,
                 tm_time.tm_min,
                 tm_time.tm_sec,
                 microseconds);
    }
    else
    {
        snprintf(czBuf, sizeof(czBuf) - 1, "%4d-%02d-%02dT%02d:%02d:%02d",
                 tm_time.tm_year + 1900,
                 tm_time.tm_mon + 1,
                 tm_time.tm_mday,
                 tm_time.tm_hour,
                 tm_time.tm_min,
                 tm_time.tm_sec);
    }
    return czBuf;
}