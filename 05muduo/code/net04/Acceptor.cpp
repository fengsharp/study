#include "Acceptor.h"

#include "Logging.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

Acceptor::Acceptor(EventLoop * pLoop, const InetAddress & listenAddr)
    : m_pLoop(pLoop)
    , m_acceptSocket(sockets::createNonblockingOrDie())
    , m_acceptChannel(m_pLoop, m_acceptSocket.fd())
    , m_newConnectionCallback(NULL)
    , m_bListenning(false)
{
    m_acceptSocket.setResuseAddr(true);
    m_acceptSocket.bindAddress(listenAddr);

    m_acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead, this, std::placeholders::_1));
}

void Acceptor::listen()
{
    m_pLoop->assertLoopInThisThread();
    m_bListenning = true;
    m_acceptSocket.listen();
    m_acceptChannel.enableReading();
}

void Acceptor::handleRead(Timestamp receiveTime)
{
    m_pLoop->assertLoopInThisThread();
    InetAddress peerAddr(0);

    int connfd = m_acceptSocket.accept(&peerAddr);
    if (connfd >= 0)
    {
        if (m_newConnectionCallback)
        {
            m_newConnectionCallback(connfd, peerAddr);
        }
        else
        {
            sockets::close(connfd);
        }
        
    }
}