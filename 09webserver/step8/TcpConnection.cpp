#include "TcpConnection.h"

#include <errno.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Logging.h"
#include "Socket.h"
#include "SocketsOpts.h"
#include "WeakCallback.h"

/*
void muduo::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
  LOG_TRACE << conn->localAddress().toIpPort() << " -> "
            << conn->peerAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
  // do not call conn->forceClose(), because some users want to register message callback only.
}

void muduo::net::defaultMessageCallback(const TcpConnectionPtr&,
                                        Buffer* buf,
                                        Timestamp)
{
  buf->retrieveAll();
}
*/

TcpConnection::TcpConnection(EventLoop * pLoop, const string & name, int sockfd, const InetAddress & localAddr, const InetAddress & peerAddr)
    : m_pLoop(pLoop)
    , m_strName(name)
    , m_state(kConnecting)
    , m_pSocket(new Socket(sockfd))
    , m_pChannel(new Channel(pLoop, sockfd))
    , m_localAddr(localAddr)
    , m_peerAddr(peerAddr)
{
    m_pChannel->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
    LOG_DEBUG << "TcpConnection::ctor[" << m_strName << "] at " << this
              << " fd=" << sockfd;
    m_pSocket->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG << "TcpConnection::dtor[" << m_strName << "] at " << this
              << " fd=" << m_pChannel->fd();
}

void TcpConnection::connectEstablished()
{
    m_pLoop->assertInLoopThread();
    assert(m_state == kConnecting);
    setState(kConnected);
    m_pChannel->tie(shared_from_this());
    m_pChannel->enableReading();

    m_connectionCallback(shared_from_this());
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
    /*
    loop_->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if (n > 0)
    {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG_SYSERR << "TcpConnection::handleRead";
        handleError();
    }
    */
    m_pLoop->assertInLoopThread();
    char buf[1024]={0};
    ssize_t n = sockets::read(m_pChannel->fd(), buf, sizeof(buf));
    m_messageCallback(shared_from_this(), buf, n);
}
