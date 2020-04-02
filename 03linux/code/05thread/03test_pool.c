#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <pthread.h>

// global num 
// if num!=0 pool pull
//    if num == -1 exit
//    else cal
// if num=0 main push
// if over main set num -1

static bool isPrim(int num);
static void* thr_work(void* arg);

#define THR_NUM 20

volatile int g_num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#define LEFT  30000000
#define RIGHT 30000200

int main()
{
    pthread_t arr[THR_NUM];
    int crtRet = -1;

    for (int i = 0; i < THR_NUM; i++)
    {
        if ((crtRet = pthread_create(arr+i, NULL, thr_work, NULL)) != 0) {
            fprintf(stderr, "pthread_create %s\n", strerror(crtRet));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= (RIGHT+1); i++)
    {
        pthread_mutex_lock(&mutex);
        while (g_num > 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        g_num = i;
        if (g_num > RIGHT) {
            g_num = -1;
            pthread_cond_broadcast(&cond);
        } else {
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);
    }
    
    for (int i = 0; i < THR_NUM; i++) {
        pthread_join(arr[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

static bool isPrim(int num) {
    bool mark = true;
    for (int j=2; j<num/2; ++j) {
        if (num % j == 0) {
            mark = false;
            break;
        }
    }
    return mark;
}

static void* thr_work(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        while (g_num == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        int num = g_num;
        if (num > 0) {
            g_num = 0;
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&mutex);

        if (g_num == -1) {
            pthread_exit(NULL);
        } else {
            if (isPrim(num)) {
                printf("%d\n", num);
            }
        }
    }
}
