#include "Channel.h"

#include <sys/epoll.h>

#include "EventLoop.h"

static const int kEventNone = 0;
static const int kEventRead = EPOLLIN | EPOLLPRI;
static const int kEventWrite = EPOLLOUT;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(kEventNone)
    , m_receiveEvents(kEventNone)
    , m_pollerState(PollerState::NEW)
{
}

bool Channel::isNoneEvents() const
{
    return m_events == kEventNone;
}

void Channel::enableReading()
{
    m_events |= kEventRead;
    update();
}

void Channel::enableWriting()
{
    m_events |= kEventWrite;
    update();
}

void Channel::disableWriting()
{
    m_events &= ~kEventWrite;
    update();
}

void Channel::disableAll()
{
    m_events = kEventNone;
    update();
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}

void Channel::handle(Timestamp receiveTime)
{
    if (m_receiveEvents & EPOLLERR)
    {
        if (m_errorEventCallback)
        {
            m_errorEventCallback();
        }
    }

    if (m_receiveEvents & kEventRead)
    {
        if (m_readEventCallback)
        {
            m_readEventCallback(receiveTime);
        }
    }

    if (m_receiveEvents & kEventWrite)
    {
        if (m_writeEventCallback)
        {
            m_writeEventCallback();
        }
    }
}