#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

#include <string>

#include "Copyable.h"

class Timestamp : public Copyable
{
public:
    static const int kMicroSecondsPerSeconds = 1 * 1000 * 1000;

    Timestamp()
        : m_microSeconds(0)
    {
    }

    explicit Timestamp(int64_t microSeconds)
        : m_microSeconds(microSeconds)
    {
    }

    void swap(Timestamp & that)
    {
        std::swap(m_microSeconds, that.m_microSeconds);
    }

    std::string toFormattedString(bool showMicroSeconds = false) const;
    bool validate() const;
    int64_t microSeconds() const;
    time_t seconds() const;

    static Timestamp now();
    static Timestamp invalid();
    static Timestamp fromUnixTime(time_t t);
    static Timestamp fromUnixTime(time_t t, int microseconds);
    // 2020-12-28T15:13:00
    static Timestamp fromStringUTC(const std::string & strUtc);

private:
    int64_t m_microSeconds;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSeconds() < rhs.microSeconds();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSeconds() == rhs.microSeconds();
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    return Timestamp(timestamp.microSeconds() + static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSeconds));
}

// seconds
inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSeconds() - low.microSeconds();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSeconds;
}

#endif