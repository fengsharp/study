#include "SocketsOps.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Logging.h"
#include "Types.h"

typedef struct sockaddr SA;

const SA * sockaddr_cast(const struct sockaddr_in * addr)
{
    return static_cast<const SA *>(implicit_cast<const void *>(addr));
}

SA * sockaddr_cast(struct sockaddr_in * addr)
{
    return static_cast<SA *>(implicit_cast<void *>(addr));
}

void setNonBlockAndCloseOnExec(int sockfd)
{
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);

    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
}

namespace sockets
{
int createNonblockingOrDie()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "create socket";
    }
    return sockfd;
}

void bindOrDie(int sockfd, const struct sockaddr_in & addr)
{
    int ret = ::bind(sockfd, sockaddr_cast(&addr), sizeof(addr));
    if (ret < 0)
    {
        LOG_SYSFATAL << "bind";
    }
}
void listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_SYSFATAL << "listen";
    }
}

int accept(int sockfd, struct sockaddr_in * addr)
{
    socklen_t addrlen = sizeof(*addr);
    int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0)
    {
        LOG_SYSFATAL << "accept";
    }
    return connfd;
}

void close(int sockfd)
{
    if (::close(sockfd) < 0)
    {
        LOG_SYSFATAL << "close";
    }
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
    addr->sin_port = sockets::hostToNetwork16(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
    {
        LOG_SYSFATAL << "fromHostPort";
    }
}

} // namespace sockes
