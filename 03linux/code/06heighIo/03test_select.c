#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

typedef enum _JobState
{
    READ,
    WRITE,
    ERR,
    EXIT,
    NONE
} JobState;

typedef struct _Job {
    int fd1;
    int fd2;
    JobState state;

    char buf[BUFSIZ];
    ssize_t nread;
} Job;

void driveJob(Job* job);

int main()
{
    int srcfd = STDIN_FILENO;
    int flags = fcntl(srcfd, F_GETFL, 0);
    fcntl(srcfd, F_SETFL, flags|O_NONBLOCK);

    int destfd = open("./test.del", O_WRONLY | O_NONBLOCK | O_TRUNC); // check
    if (destfd < 0) {
        err_exit("open dest");
    }

    Job job1;
    job1.fd1 = srcfd;
    job1.fd2 = destfd;
    job1.state = READ;

    fd_set readfds;
    fd_set writefds;
    while (job1.state != NONE) {
        if (job1.state == READ || job1.state == WRITE)
        {
            FD_ZERO(&readfds);
            FD_ZERO(&writefds);
            if (job1.state == READ)
                FD_SET(srcfd, &readfds);
            if (job1.state == WRITE)
                FD_SET(destfd, &writefds);

            int ret = select(destfd+1, &readfds, &writefds, NULL, NULL);
            if (ret < 0) {
                err_exit("select");
            }

            if (!FD_ISSET(srcfd, &readfds) && !FD_ISSET(destfd, &writefds)) {
                continue;
            }
        }
       
        driveJob(&job1); 
    }

    close(destfd);
    // close(srcfd);

    return 0;
}

void driveJob(Job* job)
{
    switch (job->state)
    {
    case READ:
        {
            job->nread = read(job->fd1, job->buf, BUFSIZ);
            if (job->nread < 0) {
                if (errno == EINTR || errno == EAGAIN) {
                    job->state = READ;
                } else {
                    job->state = ERR;
                }
            } else if (job->nread == 0) {
                job->state = EXIT;
            } else {
                static const char* EXTI_FLAG = "exit";
                if (strncmp(job->buf, EXTI_FLAG, strlen(EXTI_FLAG)) == 0) {
                    job->state = EXIT;
                } else {
                    job->state = WRITE;
                }
            }
        }
        break;
    case WRITE:
        {
            ssize_t ret = writen(job->fd2, job->buf, job->nread);
            if (ret < 0) {
                job->state = ERR;
            } else {
                job->state = READ;
            }
        }
        break;
    case ERR:
        {
            printf("--- err %d %s ---\n", errno, strerror(errno));
            job->state = EXIT;
        }
        break;
    case EXIT:
        {
            puts("--- exit, all over. ---");
            job->state = NONE;
        }
        break;
    default:
        break;
    }
}