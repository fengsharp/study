#ifndef INET_ADDRESS_H_
#define INET_ADDRESS_H_

#include <netinet/in.h>

#include "Copyable.h"
#include "StringPiece.h"
#include "SocketsOpts.h"

class InetAddress : public Copyable
{
public:
    explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

    /// Constructs an endpoint with given ip and port.
    /// @c ip should be "1.2.3.4"
    InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

    /// Constructs an endpoint with given struct @c sockaddr_in
    /// Mostly used when accepting new connections
    explicit InetAddress(const struct sockaddr_in & addr)
        : m_addr(addr)
    {
    }

    explicit InetAddress(const struct sockaddr_in6 & addr)
        : m_addr6(addr)
    {
    }

    sa_family_t family() const
    {
        return m_addr.sin_family;
    }
    string toIp() const;
    string toIpPort() const;
    uint16_t toPort() const;

    const struct sockaddr * getSockAddr() const
    {
        return sockets::sockaddr_cast(&m_addr6);
    }
    void setSockAddrInet6(const struct sockaddr_in6 & addr6)
    {
        m_addr6 = addr6;
    }

    uint32_t ipNetEndian() const;
    uint16_t portNetEndian() const
    {
        return m_addr.sin_port;
    }

    static bool resolve(StringArg hostname, InetAddress* result);
private:
    union
    {
        struct sockaddr_in m_addr;
        struct sockaddr_in6 m_addr6;
    };
};

#endif