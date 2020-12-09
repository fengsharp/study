#ifndef SOCKET_H_
#define SOCKET_H_

#include "Noncopyable.h"

class InetAddress;

class Socket : private NonCopyable
{
public:
    explicit Socket(int sockfd)
        : m_sockfd(sockfd)
    {
    }

    ~Socket();

    int fd() const
    {
        return m_sockfd;
    }

    void bindAddress(const InetAddress & localaddr);
    void listen();
    int accept(InetAddress * peerAddr);
    void setResuseAddr(bool on);

private:
    const int m_sockfd;
};

#endif