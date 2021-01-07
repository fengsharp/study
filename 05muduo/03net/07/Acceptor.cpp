#include "Acceptor.h"

#include <assert.h>

#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

Acceptor::Acceptor(EventLoop * pLoop, const InetAddress & listenAddr)
    : m_pLoop(pLoop)
    , m_acceptSocket(sockets::createNonblockingOrDie())
    , m_acceptChannel(m_pLoop, m_acceptSocket.fd())
    , m_bListenning(false)
{
    m_acceptSocket.bindAddress(listenAddr);
    m_acceptSocket.setReuseAddr(true);
    m_acceptChannel.setEventReadCallback(std::bind(&Acceptor::handleRead, this, std::placeholders::_1));

}

void Acceptor::listen()
{
    m_pLoop->assertLoopInthread();

    m_bListenning = true;
    m_acceptSocket.listen();
    m_acceptChannel.enableReading();
}

void Acceptor::handleRead(Timestamp receivedTime)
{
    m_pLoop->assertLoopInthread();

    InetAddress perrAddr(0);
    int connfd = m_acceptSocket.accept(&perrAddr);
    if (m_newConntionCallback)
    {
        m_newConntionCallback(connfd, perrAddr);
    }
    else
    {
        sockets::close(connfd);
    }
}