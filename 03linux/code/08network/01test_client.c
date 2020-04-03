#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "proto.h"

int main()
{
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sd > 0);

    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(RCVPOOT);
    int ret = inet_pton(AF_INET, "0.0.0.0", &raddr.sin_addr.s_addr);
    assert(ret == 1);

    msg_st sbuf;
    snprintf((char*)(sbuf.name), sizeof(sbuf.name), "%s", "feng");
    sbuf.math = htonl(120);
    sbuf.chinese = htonl(110);

    if (sendto(sd, &sbuf, sizeof(sbuf), 0, (struct sockaddr*)(&raddr), sizeof(raddr)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    puts("ok!");

    close(sd);

    return 0;
}