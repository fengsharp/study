#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"

int main()
{
    const char* strFileSrc = "./05test_read_write.c";
    const char* strFileDest = "./05copy.del";

    int srcfd = open(strFileSrc, O_RDONLY); // check
    int destfd = open(strFileDest, O_WRONLY | O_TRUNC | O_CREAT, 0666); // check

    ssize_t nread = 0;
    char buf[BUFSIZ];
    while ((nread = readn(srcfd, buf, BUFSIZ)) > 0) {
        writen(destfd, buf, nread);
    }

    close(destfd);
    close(srcfd);

    return 0;
}