#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "locker_pthread.h"

static volatile int g_Ticks = 100;
static Locker* g_locker = NULL;

static void* work(void* arg) {
    int* pNum = (int*)arg;

    while (1) {
        locker_lock(g_locker);
        if (g_Ticks <= 0) {
            locker_unlock(g_locker);
            break;
        }

        usleep(1 * 1000 * (rand() % 50));
        --g_Ticks;
        fprintf(stdout, "%d: buy one ticket. Now ticks: %d\n", *pNum, g_Ticks);
        locker_unlock(g_locker);

        usleep(1 * 1000 * (rand() % 100));
    }

    pthread_exit(arg);
}

#define THR_NUM 10

int main()
{
    g_locker = locker_pthread_create();

    pthread_t arrThr[THR_NUM];
    for (int i = 0; i < THR_NUM; i++)
    {
        int* pNum = malloc(sizeof(int));
        *pNum = i;
        if (pthread_create(arrThr+i, NULL, work, pNum) != 0) {
            fprintf(stderr, "pthread_create %s\n", strerror(errno));
        }
    }
    
    for (int i = 0; i < THR_NUM; i++)
    {
        void* pNum;
        pthread_join(arrThr[i], &pNum);
        free(pNum);
    }

    locker_destroy(g_locker);
    fprintf(stdout, "--- end. ticks: %d ---\n", g_Ticks);

    return 0;
}