#include <stdio.h>
#include <vector>

#include "Timestamp.h"
#include "Types.h"

void passByValue(Timestamp x);
void passByConstReference(const Timestamp & x);
void benchmark();

int main()
{
    Timestamp now(Timestamp::now());
    printf("%s\n", now.toString().c_str());
    passByValue(now);
    passByConstReference(now);
    benchmark();

    return 0;
}

void passByValue(Timestamp x)
{
    printf("passByValue:%s\n", x.toString().c_str());
}

void passByConstReference(const Timestamp & x)
{
    printf("passByConstReference:%s\n", x.toString().c_str());
}

void benchmark()
{
    const int NUMBER = 1000 * 1000;
    std::vector<Timestamp> stamps;
    stamps.reserve(NUMBER); // 预定
    for (int i = 0; i < NUMBER; i++)
    {
        stamps.push_back(Timestamp::now());
    }
    printf("start:%s\n", stamps.front().toString().c_str());
    printf("end:%s\n", stamps.back().toString().c_str());
    printf("diff:%f\n", timeDifference(stamps.back(), stamps.front()));

    int increments[100] = {0};
    memZero(increments, sizeof(increments));
    int64_t start = stamps.front().getMicroSecondsSinceEpoch();
    for (int i = 1; i < NUMBER; i++)
    {
        int64_t next = stamps[i].getMicroSecondsSinceEpoch();
        int64_t inc = next - start;
        start = next;
        if (inc < 0)
        {
            printf("reverse!\n");
        }
        else if (inc < 100)
        {
            ++increments[inc];
        }
        else
        {
            printf("big gap %d\n", static_cast<int>(inc));
        }
    }

    for (int i = 0; i < sizeof(increments) / sizeof(int); i++)
    {
        if (increments[i] > 0)
        {
            printf("%2d: %d\n", i, increments[i]);
        }
    }
}