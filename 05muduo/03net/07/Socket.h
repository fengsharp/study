#ifndef SOCKET_H_
#define SOCKET_H_

#include "NonCopyable.h"
#include "SocketsOps.h"
#include "InetAddress.h"

class Socket : private NonCopyable
{
public:
    explicit Socket(int sockfd)
        : m_sockfd(sockfd)
    {
    }

    ~Socket()
    {
        sockets::close(m_sockfd);
    }

    int fd() const
    {
        return m_sockfd;
    }

    void bindAddress(const InetAddress & localaddr)
    {
        sockets::bindOrDie(m_sockfd, localaddr.getSockaddrInet());
    }

    void listen()
    {
        sockets::listenOrDie(m_sockfd);
    }

    int accept(InetAddress * peeraddr)
    {
        struct sockaddr_in addr;
        memZero(&addr, sizeof(addr));
        int connfd = sockets::accept(m_sockfd, &addr);
        if (connfd >= 0)
        {
            peeraddr->setSockAddrInet(addr);
        }
        return connfd;
    }

    void setReuseAddr(bool on)
    {
        sockets::setReuseAddr(m_sockfd, on);
    }

private:
    const int m_sockfd;
};

#endif