#include "CircleQueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <pthread.h>


#define MAX_SIZE 30

struct _CircleQueue
{
    int head;
    int tail;
    int len;
    void* arr[MAX_SIZE];

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static bool isFull(CircleQueue* thiz) {
    return thiz->len == MAX_SIZE;
}

static bool isEmpty(CircleQueue* thiz) {
    return thiz->len == 0;
}

CircleQueue* cir_queue_create() {
    CircleQueue* pRet = malloc(sizeof(CircleQueue));
    // check
    memset(pRet, 0, sizeof(CircleQueue));
    pthread_mutex_init(&pRet->mutex, NULL); // check
    pthread_cond_init(&pRet->cond, NULL); // check
    return pRet;
}

void cir_queue_destroy(CircleQueue* thiz) {
    pthread_mutex_destroy(&thiz->mutex);
    pthread_cond_destroy(&thiz->cond);
    free(thiz);
}

void cir_queue_enqueue(CircleQueue* thiz, void* pData) {
    pthread_mutex_lock(&thiz->mutex);
    while (isFull(thiz)) {
        pthread_cond_wait(&thiz->cond, &thiz->mutex);
    }
    thiz->arr[thiz->tail] = pData;
    thiz->tail = (thiz->tail + 1) % MAX_SIZE;
    ++thiz->len;
    pthread_cond_broadcast(&thiz->cond);
    pthread_mutex_unlock(&thiz->mutex);
}

void* cir_queue_dequeue(CircleQueue* thiz){
    void* pRet = NULL;
    pthread_mutex_lock(&thiz->mutex);

    while (isEmpty(thiz)) {
        pthread_cond_wait(&thiz->cond, &thiz->mutex);
    }
    pRet = thiz->arr[thiz->head];
    thiz->head = (thiz->head + 1) % MAX_SIZE;
    --thiz->len;
    pthread_cond_broadcast(&thiz->cond);
    pthread_mutex_unlock(&thiz->mutex);
    return pRet;
}