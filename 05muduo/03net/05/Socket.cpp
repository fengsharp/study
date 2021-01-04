#include "Socket.h"

#include <netinet/in.h>
#include <netinet/tcp.h>

#include "Types.h"
#include "InetAddress.h"
#include "SocketsOpts.h"

Socket::~Socket()
{
    sockets::close(m_sockefd);
}

void Socket::bindAddress(const InetAddress & localAddr)
{
    sockets::bindOrDie(m_sockefd, localAddr.getSockAddr());
}

void Socket::listen()
{
    sockets::listenOrDie(m_sockefd);
}

int Socket::accept(InetAddress* peerAddr)
{
    struct sockaddr_in addr;
    memZero(&addr, sizeof(addr));
    int connfd = sockets::accept(m_sockefd, &addr);
    if (connfd >= 0)
    {
        peerAddr->setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockefd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}