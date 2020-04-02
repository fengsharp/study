#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#define TIME_STR_LEN 128
int main()
{
    time_t nowTime = time(NULL);
    struct tm* nowTm = localtime(&nowTime);

    char buf[TIME_STR_LEN];
    strftime(buf, TIME_STR_LEN, "%F %T", nowTm);
    puts(buf);

    return 0;
}