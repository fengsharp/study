#include "Channel.h"

#include <sys/epoll.h>

#include "EventLoop.h"

static const int kNoneEvent = 0;
static const int kReadEvent = EPOLLIN | EPOLLPRI;
// static const int kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(0)
    , m_receiveEvents(0)
    , m_status(-1)
{
}

bool Channel::isNoneEvents() const
{
    return m_events == kNoneEvent;
}

void Channel::enableReading()
{
    m_events |= kReadEvent;
    update();
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    if (m_receiveEvents & (EPOLLERR))
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_receiveEvents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (m_readCallback)
        {
            m_readCallback(receiveTime);
        }
    }

    if (m_receiveEvents & EPOLLOUT)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }
}