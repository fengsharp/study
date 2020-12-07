#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "Copyable.h"
#include "Types.h"

class Timestamp : public Copyable
{
public:
    Timestamp()
        : Timestamp(0)
    {
    }
    explicit Timestamp(int64_t microseconds)
        : m_microseconds(microseconds)
    {
    }

    std::string toString() const;
    std::string toFormattedString(bool isShowMicroseconds = true) const;

    bool valid() const
    {
        return m_microseconds > 0;
    }

    int64_t microSecondsSinceEpoch() const
    {
        return m_microseconds;
    }

    time_t secondsSinceEpoch() const
    {
        return static_cast<time_t>(m_microseconds / kMicrosecondsPerSeconds);
    }

    static Timestamp now();
    static Timestamp invalid()
    {
        return Timestamp();
    }

    static const int kMicrosecondsPerSeconds = 1000 * 1000;

private:
    int64_t m_microseconds; // 微妙
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

// seconds
inline double timeDifference(Timestamp heigh, Timestamp low)
{
    int64_t offset = heigh.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(offset) / Timestamp::kMicrosecondsPerSeconds;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    return Timestamp(timestamp.microSecondsSinceEpoch() + static_cast<int64_t>(seconds * Timestamp::kMicrosecondsPerSeconds));
}

#endif