#include "InetAddress.h"

#include "Endian.h"
#include "Logging.h"

#include <netdb.h>
#include <netinet/in.h>

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;
#pragma GCC diagnostic error "-Wold-style-cast"

//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

//     struct sockaddr_in6 {
//         sa_family_t     sin6_family;   /* address family: AF_INET6 */
//         uint16_t        sin6_port;     /* port in network byte order */
//         uint32_t        sin6_flowinfo; /* IPv6 flow information */
//         struct in6_addr sin6_addr;     /* IPv6 address */
//         uint32_t        sin6_scope_id; /* IPv6 scope-id */
//     };

static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin6_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
static_assert(offsetof(sockaddr_in6, sin6_port) == 2, "sin6_port offset 2");

InetAddress::InetAddress(uint16_t port /* = 0*/, bool loopbackOnly /* = false*/, bool ipv6 /* = false*/)
{
    static_assert(offsetof(InetAddress, m_addr) == 0, "addr_ offset 0");
    memZero(&m_addr, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
    m_addr.sin_addr.s_addr = sockets::hostToNetwork32(ip);
    m_addr.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(StringArg ip, uint16_t port, bool ipv6)
{
    memZero(&m_addr, sizeof(m_addr));
    sockets::fromIpPort(ip.data(), port, &m_addr);
}

std::string InetAddress::toIpPort() const
{
    char buf[64] = "";
    sockets::toIpPort(buf, sizeof buf, getSockAddr());
    return buf;
}

std::string InetAddress::toIp() const
{
    char buf[64] = "";
    sockets::toIp(buf, sizeof buf, getSockAddr());
    return buf;
}

uint32_t InetAddress::ipNetEndian() const
{
    assert(family() == AF_INET);
    return m_addr.sin_addr.s_addr;
}

uint16_t InetAddress::toPort() const
{
    return sockets::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(StringArg hostname, InetAddress * out)
{
    assert(out != NULL);
    struct hostent hent;
    struct hostent * he = NULL;
    int herrno = 0;
    memZero(&hent, sizeof(hent));

    int ret = gethostbyname_r(hostname.data(), &hent, t_resolveBuffer, sizeof t_resolveBuffer, &he, &herrno);
    if (ret == 0 && he != NULL)
    {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->m_addr.sin_addr = *reinterpret_cast<struct in_addr *>(he->h_addr);
        return true;
    }
    else
    {
        if (ret)
        {
            // LOG_SYSERR << "InetAddress::resolve";
        }
        return false;
    }
}
