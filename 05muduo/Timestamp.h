#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include <string>
#include <inttypes.h>

#include "copyable.h"

class Timestamp : public copyable
{
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpochArg);
    static Timestamp now();
    static Timestamp invalid() { return Timestamp(); }
    static Timestamp fromUnixTime(time_t t)
    {
        return fromUnixTime(t, 0);
    }
    static Timestamp fromUnixTime(time_t t, int microseconds)
    {
        return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
    }
    static const int kMicroSecondsPerSecond = 1000 * 1000;

    std::string toString() const;
    void swap(Timestamp& that)
    {
        std::swap(m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
    }
    bool valid() const { return m_microSecondsSinceEpoch > 0; }
    int64_t microSecondsSinceEpoch() const { return m_microSecondsSinceEpoch; }
    time_t secondsSinceEpoch() const { return static_cast<time_t>(m_microSecondsSinceEpoch / kMicroSecondsPerSecond); }
    std::string toFormattedString(bool showMicroseconds = true) const;

private:
    int64_t m_microSecondsSinceEpoch;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

#endif 