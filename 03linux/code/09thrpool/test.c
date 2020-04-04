#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "threadpool.h"

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

ThreadPool* pPool = NULL;

static void* thr_work(void* arg) {
    int* pData = (int*)(arg);
     if (isPrime(*pData)) {
        printf("%d\n", *pData);
    }
    return NULL;
}

int main()
{
    pPool = thr_pool_create();

    for (int i = LEFT; i <= RIGHT; i++)
    {
        int* pData = malloc(sizeof(int));
        *pData = i;
        thr_pool_addTask(pPool, thr_work, pData);
    }

    for (int i = 0; i < 10; i++) {
        int* pEndMark = malloc(sizeof(int));
        *pEndMark = WORK_END_FLAG;
        thr_pool_addTask(pPool, NULL, pEndMark);
    }

    thr_pool_destroy(pPool);

    return 0;
}