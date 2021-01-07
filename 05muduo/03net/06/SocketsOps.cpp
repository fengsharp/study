#include "SocketsOps.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Types.h"

namespace sockets
{

typedef struct sockaddr SA;

const SA * sockaddr_cast(const struct sockaddr_in * addr)
{
    return static_cast<const SA *>(implicit_cast<const void *>(addr));
}

SA * sockaddr_cast(struct sockaddr_in * addr)
{
    return static_cast<SA *>(implicit_cast<void *>(addr));
}

int createNonblockingOrDie()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    assert(sockfd >= 0);
    if (sockfd < 0)
    {
        perror("socket");
        abort();
    }
    return sockfd;
}

void bindOrDie(int sockfd, const struct sockaddr_in & addr)
{
    int ret = ::bind(sockfd, sockaddr_cast(&addr), sizeof(addr));
    assert(ret >= 0);
    if (ret < 0)
    {
        perror("bind");
        abort();
    }
}

void listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    assert(ret >= 0);
    if (ret < 0)
    {
        perror("listen");
        abort();
    }
}

int accept(int sockfd, struct sockaddr_in * addr)
{
    socklen_t addrlen = sizeof(sockaddr_in);
    int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    assert(connfd >= 0);
    if (connfd < 0)
    {
        perror("accept4");
        abort();
    }
    return connfd;
}

void close(int sockfd)
{
    int ret = ::close(sockfd);
    assert(ret >= 0);
    if (ret < 0)
    {
        perror("close");
        abort();
    }
}

void setReuseAddr(int sockfd, bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void toHostPort(char * buf, size_t size, const struct sockaddr_in & addr)
{
    char host[INET_ADDRSTRLEN] = "INVALID";
    ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof(host));
    uint16_t port = sockets::networkToHost16(addr.sin_port);
    snprintf(buf, size, "%s:%u", host, port);
}

void fromHostPort(const char * ip, uint16_t port, struct sockaddr_in * addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
    {
        perror("inet_pton");
        abort();
    }
}

} // namespace sockets