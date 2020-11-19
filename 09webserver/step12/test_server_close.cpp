#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "Buffer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include "Types.h"

class TestServer
{
public:
    TestServer(EventLoop * loop, const InetAddress & listenAddr)
        : m_pLoop(loop)
        , m_server(m_pLoop, listenAddr, "TestServer")
        , m_strMsg1(100, 'A')
        , m_strMsg2(200, 'B')
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
            conn->send(m_strMsg1);
            conn->send(m_strMsg2);
            conn->shutdown();
        }
        else
        {
            printf("onConnection(): connection [%s] is down\n",
                   conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr & conn,
                   Buffer * buf,
                   Timestamp receiveTime)
    {
        printf("onMessage(): received %zd bytes from connection [%s]. data:%s\n",
               buf->readableBytes(), conn->name().c_str(), buf->retrieveAllAsString().data());
    }

    EventLoop * m_pLoop;
    TcpServer m_server;

    string m_strMsg1;
    string m_strMsg2;
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