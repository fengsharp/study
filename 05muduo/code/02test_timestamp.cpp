#include <stdio.h>

#include "Timestamp.h"

int main()
{
    Timestamp now(Timestamp::now());
    printf("now: %s\n", now.toString().data());
    printf("now: %s\n", now.toFormattedString().data());
    printf("now: %s\n", now.toFormattedString(false).data());

    return 0;
}