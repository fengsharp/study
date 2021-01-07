#ifndef INET_ADDRESS_H_
#define INET_ADDRESS_H_

#include <netinet/in.h>
#include <string>

#include "Copyable.h"
#include "SocketsOps.h"
#include "Types.h"

class InetAddress : public Copyable
{
public:
    explicit InetAddress(uint16_t port)
    {
        memZero(&m_addr, sizeof(m_addr));
        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = sockets::hostToNetwork32(INADDR_ANY);
        m_addr.sin_port = sockets::hostToNetwork16(port);
    }

    InetAddress(const std::string & ip, uint16_t port)
    {
        memZero(&m_addr, sizeof(m_addr));
        sockets::fromHostPort(ip.data(), port, &m_addr);
    }

    InetAddress(const struct sockaddr_in & addr)
        : m_addr(addr)
    {
    }

    std::string toHostPort() const
    {
        // 192.168.154.135:65535 21
        char buf[32] = {0};
        sockets::toHostPort(buf, sizeof(buf) - 1, m_addr);
        return buf;
    }

    const struct sockaddr_in & getSockaddrInet() const
    {
        return m_addr;
    }

    void setSockAddrInet(const struct sockaddr_in & addr)
    {
        m_addr = addr;
    }

private:
    struct sockaddr_in m_addr;
};

#endif