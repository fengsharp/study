#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOpts.h"

void newConnection(int sockfd, const InetAddress & peerAddr)
{
    printf("newConnection(): accepted a new connection from %s\n",
           peerAddr.toIpPort().c_str());
    ::write(sockfd, "How are you?\n", 13);
    sockets::close(sockfd);
}

int main()
{
    printf("main(): pid = %d\n", getpid());

    InetAddress listenAddr(8888);
    EventLoop loop;

    Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();

    loop.loop();
}
