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

#define IPSTRSIZE 32

int main()
{
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sd > 0);

    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(RCVPOOT);
    int ret = inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    assert(ret == 1);

    ret = bind(sd, (struct sockaddr *)(&laddr), sizeof(laddr));
    assert(ret == 0);

    struct sockaddr_in raddr;
    socklen_t radrr_len = sizeof(raddr);
    msg_st rbuf;
    char ipstr[IPSTRSIZE];
    while(1) {
        recvfrom(sd, &rbuf, sizeof(rbuf), 0, (struct sockaddr *)(&raddr), &radrr_len);

        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        printf("---MESSAGE FROM %s:%d---\n", ipstr, ntohs(raddr.sin_port));
        printf("NAME=%s, math=%d, chinese=%d\n", rbuf.name, ntohl(rbuf.math), ntohl(rbuf.chinese));
    }
    
    close(sd);

    return 0;
}