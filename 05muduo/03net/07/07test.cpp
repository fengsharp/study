#include <assert.h>
#include <stdio.h>

#include <functional>
#include <string>

#include <unistd.h>

#include "Channel.h"
#include "CurrentThread.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "TcpServer.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Types.h"
#include "Callbacks.h"

EventLoop * g_pLoop = NULL;

void connectionCallback(const TcpConnectionPtr & conn)
{
    if (conn->connected())
    {
        printf("new connection %s from %s\n", conn->name().data(), conn->peerAddress().toHostPort().data());
    }
    else
    {
        printf("connection %s id down\n", conn->name().data());
    }
}

void onMessage(const TcpConnectionPtr & conn, const char * data, ssize_t len)
{
    std::string msg(data, len);
    printf("onMessage(): received msg [%s] from connection %s\n", msg.data(), conn->name().data());
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    InetAddress listenAddr(8891);
    TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(connectionCallback);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();

    return 0;
}