#ifndef TIME_ZONE_H_
#define TIME_ZONE_H_

#include "Copyable.h"
#include <memory>
#include <time.h>

class TimeZone : public Copyable
{
public:
    explicit TimeZone(const char * zonefile);
    TimeZone(int eastOfUtc, const char * tzname);
    TimeZone() = default;

    bool validate() const
    {
        return static_cast<bool>(m_data);
    }

    struct tm toLocalTime(time_t secondsSinceEpoch) const;
    time_t fromLocalTime(const struct tm &) const;

    // gmtime(3)
    static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
    // timegm(3)
    static time_t fromUtcTime(const struct tm &);
    // year in [1900..2500], month in [1..12], day in [1..31]
    static time_t fromUtcTime(int year, int month, int day,
                              int hour, int minute, int seconds);

    struct Data;

private:
    std::shared_ptr<Data> m_data;
};

#endif