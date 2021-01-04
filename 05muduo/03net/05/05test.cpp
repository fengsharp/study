#include <stdio.h>

#include <unistd.h>

#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"

void newConnection(int connfd, const InetAddress & peerAddr)
{
    printf("new connection: accepted a new connection from %s\n", peerAddr.toIpPort().data());
    ::write(connfd, "how are your?\n", 13);
    sockets::close(connfd);
}

int main()
{
    EventLoop loop;
    
    InetAddress listenAddr(9981);
    Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewCoonectionCallback(std::bind(&newConnection, std::placeholders::_1, std::placeholders::_2));
    acceptor.listen();

    loop.loop();

    puts("=== ok. ===");

    return 0;
}