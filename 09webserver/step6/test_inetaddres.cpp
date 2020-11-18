#include "InetAddress.h"
#include "Logging.h"

#include <assert.h>

//#define BOOST_TEST_MODULE InetAddressTest
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

void testInetAddress()
{
    InetAddress addr0(1234);
    assert(addr0.toIp() == string("0.0.0.0"));
    assert(addr0.toIpPort() == string("0.0.0.0:1234"));
    assert(addr0.toPort() == 1234);

    InetAddress addr1(4321, true);
    assert(addr1.toIp() == string("127.0.0.1"));
    assert(addr1.toIpPort() == string("127.0.0.1:4321"));
    assert(addr1.toPort() == 4321);

    InetAddress addr2("1.2.3.4", 8888);
    assert(addr2.toIp() == string("1.2.3.4"));
    assert(addr2.toIpPort() == string("1.2.3.4:8888"));
    assert(addr2.toPort() == 8888);

    InetAddress addr3("255.254.253.252", 65535);
    assert(addr3.toIp() == string("255.254.253.252"));
    assert(addr3.toIpPort() == string("255.254.253.252:65535"));
    assert(addr3.toPort() == 65535);
}

void testInetAddressResolve()
{
    InetAddress addr(80);
    if (InetAddress::resolve("www.baidu.com", &addr))
    {
        LOG_INFO << "www.baidu.com resolved to " << addr.toIpPort();
    }
    else
    {
        LOG_ERROR << "Unable to resolve www.baidu.com";
    }
}

int main()
{
    testInetAddress();
    testInetAddressResolve();

    return 0;
}