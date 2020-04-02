#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <pthread.h>

#define LEFT  30000000
#define RIGHT 30000200
#define THR_NUM (RIGHT - LEFT + 1)

typedef struct _ThrArg {
    int n;
} ThrArg;

void* thread_work(void* arg) {
    ThrArg* pArg = (ThrArg*)arg;
    int i = pArg->n;

    bool mark = true;
    for (int j=2; j<i/2; ++j) {
        if (i % j == 0) {
            mark = false;
            break;
        }
    }

    if (mark) {
        printf("%d\n", i);
    }

    pthread_exit(arg);
}

int main()
{
    pthread_t arr[THR_NUM];
    int ret = 0;
    
    for (int i=LEFT; i<=RIGHT; ++i) {
        ThrArg* pArg = malloc(sizeof(ThrArg));
        pArg->n = i;
        if ((ret = pthread_create(arr+(i-LEFT), NULL, thread_work, pArg)) != 0) {
            fprintf(stderr, "pthread_create %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    void* pRet = NULL;
    for (int i = 0; i < THR_NUM; i++)
    {
        pthread_join(arr[i], &pRet);
        free(pRet);
    }
    

    return 0;
}