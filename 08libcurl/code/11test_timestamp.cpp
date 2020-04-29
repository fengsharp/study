#include <stdio.h>
#include <time.h>

int main()
{
    long long now = time(NULL);
    printf("%lld\n", now);

    return 0;
}