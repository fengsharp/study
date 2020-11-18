#ifndef SOCKET_H_
#define SOCKET_H_

#include "NoneCopyable.h"

struct tcp_info;
class InetAddress;

class Socket : private NoneCopyable
{
public:
    explicit Socket(int sockfd)
        : m_sockfd(sockfd)
    {
    }

    // Socket(Socket&&) // move constructor in C++11
    ~Socket();

    int fd() const
    {
        return m_sockfd;
    }
    // return true if success.
    bool getTcpInfo(struct tcp_info *) const;
    bool getTcpInfoString(char * buf, int len) const;

    /// abort if address in use
    void bindAddress(const InetAddress & localaddr);
    /// abort if address in use
    void listen();

    /// On success, returns a non-negative integer that is
    /// a descriptor for the accepted socket, which has been
    /// set to non-blocking and close-on-exec. *peeraddr is assigned.
    /// On error, -1 is returned, and *peeraddr is untouched.
    int accept(InetAddress * peeraddr);

    void shutdownWrite();

    ///
    /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
    ///
    void setTcpNoDelay(bool on);

    ///
    /// Enable/disable SO_REUSEADDR
    ///
    void setReuseAddr(bool on);

    ///
    /// Enable/disable SO_REUSEPORT
    ///
    void setReusePort(bool on);

    ///
    /// Enable/disable SO_KEEPALIVE
    ///
    void setKeepAlive(bool on);

private:
    const int m_sockfd;
};

#endif