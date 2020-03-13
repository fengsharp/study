#include "locker_pthread.h"

#include <stdlib.h>

#include <pthread.h>

typedef struct _PrivInfo {
    pthread_mutex_t mutex;
} PrivInfo;

void locker_pthread_lock(Locker* thiz) {
    PrivInfo* pInfo = (PrivInfo*)thiz->priv;
    pthread_mutex_lock(&pInfo->mutex);
}

void locker_pthread_unlock(Locker* thiz) {
    PrivInfo* pInfo = (PrivInfo*)thiz->priv;
    pthread_mutex_unlock(&pInfo->mutex);
}

void locker_pthread_destroy(Locker* thiz) {
    PrivInfo* pInfo = (PrivInfo*)thiz->priv;
    pthread_mutex_destroy(&pInfo->mutex);
    free(thiz);
}

Locker* locker_pthread_create() {
    Locker* ret = malloc(sizeof(Locker) + sizeof(PrivInfo));
    if (ret) {
        ret->lockerFunc = locker_pthread_lock;
        ret->unlockFunc = locker_pthread_unlock;
        ret->destroyFunc = locker_pthread_destroy;

        PrivInfo* pInfo = (PrivInfo*)ret->priv;
        pthread_mutex_init(&pInfo->mutex, NULL);
    }
    return ret;
}