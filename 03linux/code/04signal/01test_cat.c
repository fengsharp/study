#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "common.h"

const int MAX_TOKEN = 50;
static volatile int token = 0;
const int SPEED = 20;

void sig_timer(int signo) {
    signal(SIGALRM, sig_timer);
    alarm(1);
    token += SPEED;
    if (token > MAX_TOKEN) {
        token = MAX_TOKEN;
    }
}

int main()
{
    signal(SIGALRM, sig_timer); // check SIG_ERR
    alarm(1);

    const char* strFileSrc = "./01test_cat.c";
    int srcfd = open(strFileSrc, O_RDONLY); // check

    char buf[BUFSIZ];
    while (true) {
        while (token <= 0) {
            pause();
        }
        ssize_t nread = readn(srcfd, buf, SPEED);
        token -= SPEED;
        if (nread <= 0) {
            break;
        }
        writen(STDOUT_FILENO, buf, nread);
    }

    close(srcfd);

    return 0;
}