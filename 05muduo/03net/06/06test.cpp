#include <assert.h>
#include <stdio.h>

#include <functional>
#include <string>

#include <unistd.h>

#include "Channel.h"
#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"
#include "Timestamp.h"
#include "Types.h"
#include "InetAddress.h"
#include "Acceptor.h"

EventLoop * g_pLoop = NULL;

void connectionCallback(int sockfd, const InetAddress & perrAddr)
{
    printf("new connection accepted a new connection from %s\n", perrAddr.toHostPort().data());
    ::write(sockfd, "how are you?\n", 13);
    sockets::close(sockfd);
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    InetAddress localAddr(8891);
    Acceptor acceptor(g_pLoop, localAddr);
    acceptor.setNewConnectionCallback(&connectionCallback);
    acceptor.listen();

    loop.loop();

    puts("=== ok. ===");

    return 0;
}