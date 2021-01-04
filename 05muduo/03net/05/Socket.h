#ifndef SOCKET_H_
#define SOCKET_H_

#include "NonCopyable.h"

class InetAddress;
class Socket : private NonCopyable
{
public:
    explicit Socket(int sockfd)
        : m_sockefd(sockfd)
    {
    }

    ~Socket();

    int fd() const
    {
        return m_sockefd;
    }

    void bindAddress(const InetAddress & localAddr);
    void listen();

    int accept(InetAddress* peerAddr);

    void setReuseAddr(bool on);

private:
    const int m_sockefd;
};

#endif