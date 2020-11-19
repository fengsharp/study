#include "TcpServer.h"

#include <stdio.h> // snprintf

#include "Acceptor.h"
#include "EventLoop.h"
#include "Logging.h"
#include "SocketsOpts.h"

TcpServer::TcpServer(EventLoop * pLoop, const InetAddress & listenAddr, const string & nameArg)
    : m_pLoop(pLoop)
    , m_strIpPort(listenAddr.toIpPort())
    , m_strName(nameArg)
    , m_pAcceptor(new Acceptor(pLoop, listenAddr))
    , m_nextConnId(1)
{
    m_pAcceptor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
    m_pLoop->assertInLoopThread();
    LOG_TRACE << "TcpServer::~TcpServer [" << m_strName << "] destructing";

    for (auto& item : m_mapConnection)
    {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    }
}

void TcpServer::start()
{
    if (m_bStarted.getAndSet(1) == 0)
    {

        assert(!m_pAcceptor->listenning());
        m_pLoop->runInLoop(std::bind(&Acceptor::listen, get_pointer(m_pAcceptor)));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress & peerAddr)
{
    m_pLoop->assertInLoopThread();
    char buf[32];
    snprintf(buf, sizeof buf, ":%s#%d", m_strIpPort.c_str(), m_nextConnId);
    ++m_nextConnId;
    string connName = m_strName + buf;

    LOG_INFO << "TcpServer::newConnection [" << m_strName
             << "] - new connection [" << connName
             << "] from " << peerAddr.toIpPort();
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(m_pLoop,
                                            connName,
                                            sockfd,
                                            localAddr,
                                            peerAddr));
    m_mapConnection[connName] = conn;
    conn->setConnectionCallback(m_connectionCallback);
    conn->setMessageCallback(m_messageCallback);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));
    conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    m_pLoop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    m_pLoop->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnectionInLoop [" << m_strName
           << "] - connection " << conn->name();
    
    size_t n = m_mapConnection.erase(conn->name());
    (void)n;
    assert(n == 1);
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

