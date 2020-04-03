#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <pthread.h>

#include "CircleQueue.h"
#include "common.h"

/*
一个线程发送数据
n个线程处理数据
*/

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

static CircleQueue* g_queue;
#define THR_NUM 20
#define WORK_END_FLAG -1
static void* thr_work(void* arg);
int main()
{
    g_queue = cir_queue_create();

    pthread_t arr[THR_NUM];
    int crtRet = -1;
    for (int i = 0; i < THR_NUM; i++)
    {
        if ((crtRet = pthread_create(arr+i, NULL, thr_work, NULL)) != 0) {
            fprintf(stderr, "pthread_create %s\n", strerror(crtRet));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= RIGHT; i++)
    {
        int* pData = malloc(sizeof(int));
        *pData = i;
        cir_queue_enqueue(g_queue, pData);
    }

    for (int i = 0; i < THR_NUM; i++) {
        int* pEndMark = malloc(sizeof(int));
        *pEndMark = WORK_END_FLAG;
        cir_queue_enqueue(g_queue, pEndMark);
    }

    for (int i = 0; i < THR_NUM; i++) {
        pthread_join(arr[i], NULL);
    }

    cir_queue_destroy(g_queue);

    return 0;
}

static void* thr_work(void* arg) {
    while (true) {
        int* pData = cir_queue_dequeue(g_queue);
        if (*pData == WORK_END_FLAG) {
            SAFE_FREE(pData);
            break;
        } else {
            if (isPrime(*pData)) {
                printf("%d\n", *pData);
            }
            SAFE_FREE(pData);
        }
    }
    pthread_exit(NULL);
}

/*
real    0m2.173s
user    0m2.146s
sys     0m0.012s
*/