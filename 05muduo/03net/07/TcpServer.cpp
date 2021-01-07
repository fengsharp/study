#include "TcpServer.h"

#include <assert.h>

#include "EventLoop.h"
#include "SocketsOps.h"

TcpServer::TcpServer(EventLoop * pLoop, const InetAddress & listenAddr)
    : m_pLoop(pLoop)
    , m_name(listenAddr.toHostPort())
    , m_acceptor(m_pLoop, listenAddr)
    , m_bStarted(false)
    , m_nextConnId(1)
{
    m_acceptor.setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    if (m_bStarted)
    {
        return;
    }

    m_bStarted = true;
    m_pLoop->runInLoop(std::bind(&Acceptor::listen, &m_acceptor));
}

void TcpServer::newConnection(int sockfd, const InetAddress & peerAddr)
{
    m_pLoop->assertLoopInthread();

    char buf[32];
    snprintf(buf, sizeof(buf), "#%d", m_nextConnId);
    ++m_nextConnId;
    std::string connName = m_name + buf;

    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr pConn = std::make_shared<TcpConnection>(m_pLoop, connName, sockfd, localAddr, peerAddr);
    m_mapConnections[connName] = pConn;

    pConn->setConnectionCallback(m_connectionCallback);
    pConn->setMessageCallback(m_messageCallback);
    pConn->connectEstablished();
}