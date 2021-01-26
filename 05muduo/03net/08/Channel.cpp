#include "Channel.h"

#include <sys/epoll.h>

#include "EventLoop.h"
#include "Poller.h"

static const int kNoneEvent = 0;
static const int kReadEvent = EPOLLIN | EPOLLPRI;
static const int kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_status(PollerStatus::NEW)
    , m_events(kNoneEvent)
    , m_receivedEvents(kNoneEvent)
{
}

bool Channel::isNoneEvents() const
{
    return m_events == kNoneEvent;
}

void Channel::enableReading()
{
    m_events |= kReadEvent;
    updateChannel();
}

void Channel::enableWriting()
{
    m_events |= kWriteEvent;
    updateChannel();
}

void Channel::disableWriting()
{
    m_events &= ~kWriteEvent;
    updateChannel();
}

void Channel::disableAll()
{
    m_events = kNoneEvent;
    updateChannel();
}

void Channel::updateChannel()
{
    m_pLoop->updateChannel(this);
}

void Channel::handleEvent()
{
    if (m_receivedEvents & (EPOLLERR))
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_receivedEvents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (m_readCallback)
        {
            m_readCallback();
        }
    }

    if (m_receivedEvents & EPOLLOUT)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }
}