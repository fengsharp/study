#include "threadpool.h"

#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#define MAX_THR_NUM 10

struct _ThreadPool
{
    Task* pTaskHead;
    Task* pTaskTail;
    int taskLen;

    int thrIdle;
    int thrCnt;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

ThreadPool* thr_pool_create()
{
    ThreadPool* pRet = malloc(sizeof(ThreadPool)); // check
    memset(pRet, 0, sizeof(ThreadPool)); 
    pthread_mutex_init(&pRet->mutex, NULL); // check
    pthread_cond_init(&pRet->cond, NULL); // check
    return pRet;
}

void thr_pool_destroy(ThreadPool* thiz)
{
    pthread_mutex_lock(&thiz->mutex);
    while (thiz->thrCnt > 0) {
        pthread_cond_wait(&thiz->cond, &thiz->mutex);
    }
    pthread_mutex_unlock(&thiz->mutex);

    pthread_mutex_destroy(&thiz->mutex);
    pthread_cond_destroy(&thiz->cond);
    free(thiz);
}

static void* thr_work(void* arg);

void thr_pool_addTask(ThreadPool* thiz, RunFunc runCallback, void* arg)
{
    Task* pTask = malloc(sizeof(Task));
    pTask->arg = arg;
    pTask->runCallback = runCallback;
    pTask->pNext = NULL;

    pthread_mutex_lock(&thiz->mutex);
    if (thiz->pTaskHead == NULL) {
        thiz->pTaskHead = pTask;
    } else {
        thiz->pTaskTail->pNext = pTask;
    }
    thiz->pTaskTail = pTask;
    thiz->taskLen += 1;

    if (thiz->thrIdle > 0) {
        pthread_cond_signal(&thiz->cond);
    } else if (thiz->thrCnt < MAX_THR_NUM) {
        // add thread
        pthread_t pthid;
        pthread_create(&pthid, NULL, thr_work, thiz); // check
        ++thiz->thrCnt;
    }

    pthread_mutex_unlock(&thiz->mutex);
}

static void* thr_work(void* arg)
{
    ThreadPool* pPool = (ThreadPool*)(arg);
    while (1) {
        Task* pTask = NULL;
        pthread_mutex_lock(&pPool->mutex);
        ++pPool->thrIdle;
        while (pPool->taskLen == 0) {
            pthread_cond_wait(&pPool->cond, &pPool->mutex);
        }
        --pPool->thrIdle;

        pTask = pPool->pTaskHead;
        pPool->pTaskHead = pPool->pTaskHead->pNext;
        --pPool->taskLen;

        pthread_mutex_unlock(&pPool->mutex);

        int* pValue = (int*)(pTask->arg);
        if (pTask->runCallback != NULL) {
            pTask->runCallback(pTask->arg);
        }
        free(pTask);

        if (*pValue == WORK_END_FLAG) {
            break;
        }
    }

    pthread_mutex_lock(&pPool->mutex);
    --pPool->thrCnt;
    pthread_cond_signal(&pPool->cond);
    pthread_mutex_unlock(&pPool->mutex);
    pthread_exit(NULL);
}