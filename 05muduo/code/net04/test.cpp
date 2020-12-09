#include <functional>
#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Acceptor.h"
#include "SocketsOps.h"
#include "InetAddress.h"

EventLoop * g_loop;

void newConnection(int sockfd, const InetAddress & peerAddr)
{
    printf("pid=%d accepted a new connection: %s\n", getpid(), peerAddr.toHostPort().data());
    ::write(sockfd, "How are you?\n", 13);
    sockets::close(sockfd);

    g_loop->quit();
}

int main()
{
    printf("main(): pid=%d\n", getpid());

    EventLoop loop;
    g_loop = &loop;
    
    InetAddress address(9981);
    Acceptor acceptor(&loop, address);
    acceptor.setNewConnectionCallback(std::bind(newConnection, std::placeholders::_1, std::placeholders::_2));
    acceptor.listen();

    loop.loop();

    puts("=== over. ===");

    return 0;
}