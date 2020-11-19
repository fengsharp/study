#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"

class TestServer
{
public:
    TestServer(EventLoop * loop,
               const InetAddress & listenAddr)
        : m_pLoop(loop)
        , m_server(m_pLoop, listenAddr, "TestServer")
    {
        m_server.setConnectionCallback(
            std::bind(&TestServer::onConnection, this, _1));
        m_server.setMessageCallback(
            std::bind(&TestServer::onMessage, this, _1, _2, _3));
    }

    void start()
    {
        m_server.start();
    }

private:
    void onConnection(const TcpConnectionPtr & conn)
    {
        if (conn->connected())
        {
            printf("onConnection(): new connection [%s] from %s\n",
                   conn->name().c_str(),
                   conn->getPeerAddress().toIpPort().c_str());
        }
        else
        {
            printf("onConnection(): connection [%s] is down\n",
                   conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr & conn,
                   const char * data,
                   ssize_t len)
    {
        printf("onMessage(): received %zd bytes from connection [%s]. data:%s\n",
               len, conn->name().c_str(), data);
    }

    EventLoop * m_pLoop;
    TcpServer m_server;
};

int main()
{
    printf("main(): pid = %d\n", getpid());

    InetAddress listenAddr(8888);
    EventLoop loop;

    TestServer server(&loop, listenAddr);
    server.start();

    loop.loop();
}