#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define THR_NUM 20

static const char* FILE_PATH = "a.txt";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* thread_work(void* arg) {
    FILE* pFile = fopen(FILE_PATH, "r+");
    if (pFile == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buf[BUFSIZ];

    pthread_mutex_lock(&mutex);
    fgets(buf, BUFSIZ, pFile);
    int n = atoi(buf);
    fseek(pFile, 0, SEEK_SET);
    usleep(100);
    fprintf(pFile, "%d", n+1);
    fclose(pFile);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main()
{
    int fd = -1;
    if ((fd = open(FILE_PATH, O_RDONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    } else {
        close(fd);
    }

    pthread_t arr[THR_NUM];
    int crtRet = -1;

    for (int i = 0; i < THR_NUM; i++)
    {
        if ((crtRet = pthread_create(arr+i, NULL, thread_work, NULL)) != 0) {
            fprintf(stderr, "pthread_create %s\n", strerror(crtRet));
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < THR_NUM; i++) {
        pthread_join(arr[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}