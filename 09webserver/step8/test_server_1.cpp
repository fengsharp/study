#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"

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

int main()
{
    printf("main(): pid = %d\n", getpid());

    InetAddress listenAddr(8888);
    EventLoop loop;

    TcpServer server(&loop, listenAddr, "TestServer");
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}
