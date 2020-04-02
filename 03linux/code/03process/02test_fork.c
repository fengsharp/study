#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "common.h"

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
        pid_t pid = fork();
        if (pid < 0) {
            err_exit("frok");
        }
        else if (pid == 0) {
            if (isPrime(i)) {
                printf("%d\n", i);
            }
            exit(EXIT_SUCCESS);
        } else {

        }
    }

    for (int i = LEFT; i <= RIGHT; ++i) {
        wait(NULL);
    }

    return 0;
}

/*
real    0m2.227s
user    0m2.180s
sys     0m0.031s
*/