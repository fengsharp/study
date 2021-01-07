#include "TcpConnection.h"

#include <assert.h>
#include <unistd.h>

#include "EventLoop.h"

TcpConnection::TcpConnection(EventLoop * pLoop, const std::string & name, int sockfd, const InetAddress & localAddr, const InetAddress & peerAddr)
    : m_pLoop(pLoop)
    , m_connSocket(sockfd)
    , m_connChannel(m_pLoop, sockfd)
    , m_name(name)
    , m_state(StateE::kConnecting)
    , m_localAddr(localAddr)
    , m_peerAddr(peerAddr)
{
    m_connChannel.setEventReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::connectEstablished()
{
    m_pLoop->assertLoopInthread();

    assert(m_state == StateE::kConnecting);
    setState(StateE::kConnected);

    m_connChannel.enableReading();

    if (m_connectionCallback)
    {
        m_connectionCallback(shared_from_this());
    }
}

void TcpConnection::handleRead(Timestamp receivedTime)
{
    char buf[1024];
    ssize_t n = ::read(m_connChannel.fd(), buf, sizeof(buf));
    if (m_messageCallback)
    {
        m_messageCallback(shared_from_this(), buf, n);
    }
}