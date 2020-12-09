#include "InetAddress.h"

#include <assert.h>

#include <netinet/in.h>
#include <strings.h>

#include "SocketsOps.h"

static const in_addr_t kInaddrAny = INADDR_ANY;

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
    bzero(&m_addr, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = sockets::hostToNetwork32(kInaddrAny);
    m_addr.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string & ip, uint16_t port)
{
    bzero(&m_addr, sizeof(m_addr));
    sockets::fromHostPort(ip.data(), port, &m_addr);
}

std::string InetAddress::toHostPort() const
{
    char buf[32];
    sockets::toHostPort(buf, sizeof(buf), m_addr);
    return buf;
}