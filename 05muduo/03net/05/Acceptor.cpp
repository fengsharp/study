#include "Acceptor.h"

#include "EventLoop.h"
#include "InetAddress.h"

Acceptor::Acceptor(EventLoop * pLoop, const InetAddress & listenAddr)
    : m_pLoop(pLoop)
    , m_acceptSocket(sockets::createNonblockingOrDie(listenAddr.family()))
    , m_acceptChannel(m_pLoop, m_acceptSocket.fd())
    , m_bListenning(false)
{
    m_acceptSocket.setReuseAddr(true);
    m_acceptSocket.bindAddress(listenAddr);
    m_acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead, this, std::placeholders::_1));
}

void Acceptor::listen()
{
    m_pLoop->assertLoopInThread();

    m_bListenning = true;
    m_acceptSocket.listen();
    m_acceptChannel.enableReading();
}

void Acceptor::handleRead(Timestamp receivedTime)
{
    m_pLoop->assertLoopInThread();

    InetAddress perrAddr { 0 };
    int confd = m_acceptSocket.accept(&perrAddr);
    if (confd >= 0)
    {
        if (m_newCoonectionCallback)
        {
            m_newCoonectionCallback(confd, perrAddr);
        }
        else
        {
            sockets::close(confd);
        }
        
    }
}