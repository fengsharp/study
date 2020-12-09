#include "Socket.h"

#include <strings.h>

#include "InetAddress.h"
#include "SocketsOps.h"

Socket::~Socket()
{
    sockets::close(m_sockfd);
}

void Socket::bindAddress(const InetAddress & localaddr)
{
    sockets::bindOrDie(m_sockfd, localaddr.getSockAddrInet());
}

void Socket::listen()
{
    sockets::listenOrDie(m_sockfd);
}

int Socket::accept(InetAddress * peerAddr)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    int connfd = sockets::accept(m_sockfd, &addr);
    if (connfd >= 0)
    {
        peerAddr->setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::setResuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}