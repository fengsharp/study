#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "Copyable.h"
#include "Types.h"

/*
 utc, microseconds
*/
class Timestamp : public Copyable
{
public:
    Timestamp();
    explicit Timestamp(int64_t microSeconds);
    void swap(Timestamp& that);
    string toString() const;
    string toFormattedString(bool showMicroSeconds = true) const;

    bool isValid() const;

    int64_t getMicroSecondsSinceEpoch() const;
    time_t getSecondsSinceEpoch() const;

    static Timestamp now();
    static Timestamp invalid();

    static Timestamp fromUnixTime(time_t t);
    static Timestamp fromUnixTime(time_t t, int microseconds);

    static const int MICROSECONDS_PER_SECONDS = 1000 * 1000;
private:
    int64_t m_i64MicroSeconds;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.getMicroSecondsSinceEpoch() < rhs.getMicroSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.getMicroSecondsSinceEpoch() == rhs.getMicroSecondsSinceEpoch();
}

// return unit: seconds
inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.getMicroSecondsSinceEpoch() - low.getMicroSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::MICROSECONDS_PER_SECONDS;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::MICROSECONDS_PER_SECONDS);
    return Timestamp(timestamp.getMicroSecondsSinceEpoch() + delta);
}

// seconds millisecond  microseconds

#endif