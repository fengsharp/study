#include "Acceptor.h"

#include "EventLoop.h"
#include "InetAddress.h"
#include "Logging.h"
#include "Socket.h"
#include "SocketsOpts.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop * pLoop, const InetAddress & listenAddr, bool reuseport/* = false*/)
    : m_pLoop(pLoop)
    , m_acceptSocket(sockets::createNonblockingOrDie(listenAddr.family()))
    , m_acceptChannel(pLoop, m_acceptSocket.fd())
    , m_bListing(false)
    , m_idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    assert(m_idleFd >= 0);
    m_acceptSocket.setReuseAddr(true);
    m_acceptSocket.setReusePort(reuseport);
    m_acceptSocket.bindAddress(listenAddr);
    m_acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    m_acceptChannel.disableAll();
    m_acceptChannel.remove();
    ::close(m_idleFd);
}

void Acceptor::listen()
{
    m_pLoop->assertInLoopThread();
    m_bListing = true;
    m_acceptSocket.listen();
    m_acceptChannel.enableReading();
}

void Acceptor::handleRead()
{
    m_pLoop->assertInLoopThread();

    InetAddress perrAddr;
    int connfd = m_acceptSocket.accept(&perrAddr);
    if (connfd >= 0)
    {
        if (m_newConnectionCallback)
        {
            m_newConnectionCallback(connfd, perrAddr);
        }
        else
        {
            sockets::close(connfd);
        }
    }
    else
    {
        LOG_SYSERR << "in Acceptor::handleRead()";
        if (errno == EMFILE)
        {
            ::close(m_idleFd);
            m_idleFd = ::accept(m_acceptSocket.fd(), NULL, NULL);
            ::close(m_idleFd);
            m_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}