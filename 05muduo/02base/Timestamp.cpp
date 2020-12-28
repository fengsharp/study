#include "Timestamp.h"

#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "Types.h"

std::string Timestamp::toFormattedString(bool showMicroSeconds /* = false*/) const
{
    time_t timeSeconds = seconds();
    struct tm tmValue;
    gmtime_r(&timeSeconds, &tmValue);

    char buf[64] = { 0 };
    if (showMicroSeconds)
    {
        int microseconds = static_cast<int>(m_microSeconds % kMicroSecondsPerSeconds);

        snprintf(buf, sizeof(buf) - 1, "%04d-%02d-%02dT%02d:%02d:%02d.%06d",
                 tmValue.tm_year + 1900, tmValue.tm_mon + 1, tmValue.tm_mday,
                 tmValue.tm_hour, tmValue.tm_min, tmValue.tm_sec, microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf) - 1, "%04d-%02d-%02dT%02d:%02d:%02d",
                 tmValue.tm_year + 1900, tmValue.tm_mon + 1, tmValue.tm_mday,
                 tmValue.tm_hour, tmValue.tm_min, tmValue.tm_sec);
    }
    return buf;
}

bool Timestamp::validate() const
{
    return m_microSeconds > 0;
}

int64_t Timestamp::microSeconds() const
{
    return m_microSeconds;
}

time_t Timestamp::seconds() const
{
    return static_cast<time_t>(m_microSeconds / kMicroSecondsPerSeconds);
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return Timestamp(tv.tv_sec * kMicroSecondsPerSeconds + tv.tv_usec);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}

Timestamp Timestamp::fromUnixTime(time_t t)
{
    return fromUnixTime(t, 0);
}

Timestamp Timestamp::fromUnixTime(time_t t, int microSeconds)
{
    return Timestamp(static_cast<int64_t>(t * kMicroSecondsPerSeconds) + implicit_cast<int64_t>(microSeconds));
}

// 2020-12-28T15:13:00
Timestamp Timestamp::fromStringUTC(const std::string & strUtc)
{
    struct tm tmTime;
    char * ret = strptime(strUtc.data(), "%Y-%m-%dT%H:%M:%S", &tmTime);
    assert(ret != NULL);
    time_t timestamp = mktime(&tmTime);
    return Timestamp(static_cast<int64_t>(timestamp * kMicroSecondsPerSeconds));
}   