#include "Channel.h"

#include <assert.h>
#include <sys/epoll.h>

#include "EventLoop.h"
#include "Poller.h"

static const int kEventNone = 0;
static const int kEventRead = EPOLLIN | EPOLLRDHUP | EPOLLPRI;
static const int kEventWrite = EPOLLOUT;
static const int kEventError = EPOLLERR;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(kEventNone)
    , m_receiveEvents(kEventNone)
    , m_pollerStatus(ChannelPollerStatus::NEW)
{
}

void Channel::enableRead()
{
    m_events |= kEventRead;
    update();
}

void Channel::enableWrite()
{
    m_events |= kEventWrite;
}

void Channel::removeWrite()
{
    m_events &= ~kEventWrite;
}

void Channel::enableError()
{
    m_events |= kEventError;
}

void Channel::clearAllEvents()
{
    m_events = kEventNone;
}

bool Channel::hasNoneEvents() const
{
    return m_events == kEventNone;
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}

void Channel::handle(Timestamp receivedTime)
{
    if (m_receiveEvents & kEventError)
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_receiveEvents & kEventRead)
    {
        if (m_readCallback)
        {
            m_readCallback(receivedTime);
        }
    }

    if (m_receiveEvents & kEventWrite)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }
}










