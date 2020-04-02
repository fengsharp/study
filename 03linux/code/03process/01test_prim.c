#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEFT  30000000
#define RIGHT 30000200
bool isPrime(int num) {
    for (int i = 2; i < num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int main()
{
    for (int i = LEFT; i <= RIGHT; ++i) {
        if (isPrime(i)) {
            printf("%d\n", i);
        }
    }

    return 0;
}

/*
real    0m2.169s
user    0m2.154s
sys     0m0.000s
*/