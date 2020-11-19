#include "TcpConnection.h"

#include <errno.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Logging.h"
#include "Socket.h"
#include "SocketsOpts.h"
#include "WeakCallback.h"

void defaultConnectionCallback(const TcpConnectionPtr & conn)
{
    LOG_TRACE << conn->getLocalAddress().toIpPort() << " -> "
              << conn->getPeerAddress().toIpPort() << " is "
              << (conn->connected() ? "UP" : "DOWN");
    // do not call conn->forceClose(), because some users want to register message callback only.
}

void defaultMessageCallback(const TcpConnectionPtr &, Buffer * buf, Timestamp)
{
    buf->retrieveAll();
}

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
    m_pChannel->setCloseCallbck(std::bind(&TcpConnection::handleClose, this));
    m_pChannel->setErrorCallbck(std::bind(&TcpConnection::handleError, this));
    LOG_DEBUG << "TcpConnection::ctor[" << m_strName << "] at " << this
              << " fd=" << sockfd;
    m_pSocket->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG << "TcpConnection::dtor[" << m_strName << "] at " << this
              << " fd=" << m_pChannel->fd();
}

void TcpConnection::send(const void * data, int len)
{
    send(StringPiece(static_cast<const char *>(data), len));
}

void TcpConnection::send(const StringPiece & message)
{
    if (m_state == kConnected)
    {
        if (m_pLoop->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            void (TcpConnection::*fp)(const StringPiece & message) = &TcpConnection::sendInLoop;
            m_pLoop->runInLoop(
                std::bind(fp,
                          this, // FIXME
                          message));
            //std::forward<string>(message)));
        }
    }
}

// FIXME efficiency!!!
void TcpConnection::send(Buffer * buf)
{
    if (m_state == kConnected)
    {
        if (m_pLoop->isInLoopThread())
        {
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        }
        else
        {
            void (TcpConnection::*fp)(const StringPiece & message) = &TcpConnection::sendInLoop;
            m_pLoop->runInLoop(
                std::bind(fp,
                          this, // FIXME
                          std::forward<string>(buf->retrieveAllAsString())));
        }
    }
}

void TcpConnection::sendInLoop(const StringPiece & message)
{
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void * data, size_t len)
{
    m_pLoop->assertInLoopThread();
    sockets::write(m_pChannel->fd(), data, len);

    /*
    m_pLoop->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if (m_state == kDisconnected)
    {
        LOG_WARN << "disconnected, give up writing";
        return;
    }
    // if no thing in output queue, try writing directly
    if (!m_pChannel->isWriting() && m_outputBuffer.readableBytes() == 0)
    {
        nwrote = sockets::write(m_pChannel->fd(), data, len);
        if (nwrote >= 0)
        {
            remaining = len - nwrote;
            if (remaining == 0 && )
            {
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        }
        else // nwrote < 0
        {
            nwrote = 0;
            if (errno != EWOULDBLOCK)
            {
                LOG_SYSERR << "TcpConnection::sendInLoop";
                if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
                {
                    faultError = true;
                }
            }
        }
    }

    assert(remaining <= len);
    if (!faultError && remaining > 0)
    {
        size_t oldLen = outputBuffer_.readableBytes();
        if (oldLen + remaining >= highWaterMark_
            && oldLen < highWaterMark_
            && highWaterMarkCallback_)
        {
            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
        }
        outputBuffer_.append(static_cast<const char *>(data) + nwrote, remaining);
        if (!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
    */
}

void TcpConnection::shutdown()
{
    // FIXME: use compare and swap
    if (m_state == kConnected)
    {
        setState(kDisconnecting);
        // FIXME: shared_from_this()?
        m_pLoop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop()
{
    m_pLoop->assertInLoopThread();
    if (!m_pChannel->isWriting())
    {
        // we are not writing
        m_pSocket->shutdownWrite();
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    m_pSocket->setTcpNoDelay(on);
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

void TcpConnection::connectDestroyed()
{
    m_pLoop->assertInLoopThread();
    if (m_state == kConnected)
    {
        setState(kDisconnected);
        m_pChannel->disableAll();
        m_connectionCallback(shared_from_this());
    }
    m_pChannel->remove();
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
    m_pLoop->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = m_inputBuffer.readFd(m_pChannel->fd(), &savedErrno);
    if (n > 0)
    {
        m_messageCallback(shared_from_this(), &m_inputBuffer, receiveTime);
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
}

void TcpConnection::handleClose()
{
    m_pLoop->assertInLoopThread();
    LOG_TRACE << "fd = " << m_pChannel->fd() << " state = " << m_state;
    assert(m_state == kConnected || m_state == kDisconnecting);
    // we don't close fd, leave it to dtor, so we can find leaks easily.
    setState(kDisconnected);
    m_pChannel->disableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    m_connectionCallback(guardThis); // 这一行，可以不调用
    // must be the last line
    m_closeCallback(guardThis); // 调用TcpServer::removeConnection
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(m_pChannel->fd());
    LOG_ERROR << "TcpConnection::handleError [" << m_strName
              << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}
