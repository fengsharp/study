#include "Timestamp.h"

#include <stdio.h>
#include <sys/time.h>

Timestamp::Timestamp()
    : m_i64MicroSeconds(0)
{
}

Timestamp::Timestamp(int64_t microSeconds)
    : m_i64MicroSeconds(microSeconds)
{
}

void Timestamp::swap(Timestamp & that)
{
    std::swap(m_i64MicroSeconds, that.m_i64MicroSeconds);
}

string Timestamp::toString() const
{
    char buf[32] = { 0 };
    int64_t seconds = m_i64MicroSeconds / MICROSECONDS_PER_SECONDS;
    int64_t microseconds = m_i64MicroSeconds % MICROSECONDS_PER_SECONDS;
    snprintf(buf, sizeof(buf) - 1, "%ld.%06ld", seconds, microseconds);
    return buf;
}

string Timestamp::toFormattedString(bool showMicroSeconds /* = true*/) const
{
    char buf[64] = { 0 };
    time_t seconds = static_cast<time_t>(m_i64MicroSeconds / MICROSECONDS_PER_SECONDS);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    if (showMicroSeconds)
    {
        int microseconds = static_cast<int>(m_i64MicroSeconds % MICROSECONDS_PER_SECONDS);
        snprintf(buf, sizeof(buf) - 1, "%04d-%02d-%02dT%02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1990, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                 microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf) - 1, "%04d-%02d-%02dT%02d:%02d:%02d",
                 tm_time.tm_year + 1990, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}

bool Timestamp::isValid() const
{
    return m_i64MicroSeconds > 0;
}

int64_t Timestamp::getMicroSecondsSinceEpoch() const
{
    return m_i64MicroSeconds;
}

time_t Timestamp::getSecondsSinceEpoch() const
{
    return static_cast<time_t>(m_i64MicroSeconds / MICROSECONDS_PER_SECONDS);
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return Timestamp(tv.tv_sec * MICROSECONDS_PER_SECONDS + tv.tv_usec);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}

Timestamp Timestamp::fromUnixTime(time_t t)
{
    return fromUnixTime(t, 0);
}

Timestamp Timestamp::fromUnixTime(time_t t, int microseconds)
{
    return Timestamp(static_cast<int64_t>(t) * MICROSECONDS_PER_SECONDS + microseconds);
}
