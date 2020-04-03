#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"

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

void driverJobs(Job* job1, Job* job2);
void driveJob(Job* job);

int main()
{
    int srcfd = open(TTY1, O_RDWR | O_NONBLOCK); // check
    int destfd = open(TTY2, O_RDWR | O_NONBLOCK); // check

    Job job1;
    job1.fd1 = srcfd;
    job1.fd2 = destfd;
    job1.state = READ;

    Job job2;
    job2.fd1 = destfd;
    job2.fd2 = srcfd;
    job2.state = READ;

    driverJobs(&job1, &job2);

    close(destfd);
    close(srcfd);

    return 0;
}

void driverJobs(Job* job1, Job* job2)
{
    while (job1->state != EXIT || job2->state != EXIT) {
        driveJob(job1);
        driveJob(job2);
    }
}

void driveJob(Job* job)
{
    switch (job->state)
    {
    case READ:
        {
            job->nread = readn(job->fd1, job->buf, BUFSIZ);
            if (job->nread < 0) {
                job->state = ERR;
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
            // fprintf(job->fd1, "--- err %s ---\n", strerror(errno));
            job->state = EXIT;
        }
        break;
    case EXIT:
        {
            // fprintf(job->fd1, "--- exit, all over. ---\n");
            job->state = NONE;
        }
        break;
    default:
        break;
    }
}