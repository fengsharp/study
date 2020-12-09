#ifndef INET_ADDRESS_H_
#define INET_ADDRESS_H_

#include <netinet/in.h>
#include <string>

#include "Copyable.h"

class InetAddress : public Copyable
{
public:
    explicit InetAddress(uint16_t port);
    InetAddress(const std::string & ip, uint16_t port);

    InetAddress(const struct sockaddr_in & addr)
        : m_addr(addr)
    {
    }

    std::string toHostPort() const;
    const struct sockaddr_in & getSockAddrInet() const
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