#include <stdio.h>

#include "Timestamp.h"

int main()
{
    Timestamp now = Timestamp::now();
    puts(now.toFormattedString().data());

    const char * timeUtc = "2020-04-23T08:24:00.000Z";
    Timestamp tmp = Timestamp::fromStringUTC(timeUtc);
    puts(tmp.toFormattedString().data());

    timeUtc = "2020-04-23T08:24:00";
    tmp = Timestamp::fromStringUTC(timeUtc);
    puts(tmp.toFormattedString().data());

    Timestamp nextDay = addTime(now, 1 * 24 * 60 * 60);
    puts(nextDay.toFormattedString().data());

    double diff = timeDifference(nextDay, now);
    printf("diff %f\n", diff);

    return 0;
}