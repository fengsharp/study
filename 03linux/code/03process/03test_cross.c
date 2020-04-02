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
#define PROC_NUM 20

bool isPrime(int num) {
    for (int i = 2; i < num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void processWork(int processID) {
    for (int i = LEFT + processID; i <= RIGHT; i += PROC_NUM) {
        if (isPrime(i)) {
            printf("%d\n", i);
        }   
    }
}

int main()
{
    for (int i=0; i<PROC_NUM; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            err_exit("frok");
        }
        else if (pid == 0) {
            processWork(i);
            exit(EXIT_SUCCESS);
        } else {

        }
    }

    for (int i=0; i<PROC_NUM; ++i) {
        wait(NULL);
    }

    return 0;
}

/*
real    0m2.182s
user    0m2.165s
sys     0m0.002s
*/