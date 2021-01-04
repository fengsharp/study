#include "Channel.h"

#include <assert.h>
#include <sys/epoll.h>

#include "EventLoop.h"
#include "Poller.h"

/*
EPOLLIN  EPOLLRDHUP EPOLLPRI
EPOLLOUT
EPOLLERR
*/
static const int kEpollEventNone = 0;
static const int kEpollEventRead = EPOLLIN | EPOLLRDHUP | EPOLLPRI;
static const int kEpollEventWrite = EPOLLOUT;
static const int kEpollEventError = EPOLLERR;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_pollerStatus(PollerStatus::NEW)
    , m_receivedEvents(kEpollEventNone)
    , m_events(kEpollEventNone)
{
}

void Channel::setReciveEvents(int events)
{
    m_receivedEvents = events;
}

bool Channel::hasNoneEvents() const
{
    return m_events == kEpollEventNone;
}

void Channel::enableReading()
{
    m_events |= kEpollEventRead;
    update();
}

void Channel::enableWriting()
{
    m_events |= kEpollEventWrite;
    update();
}

void Channel::removeWriting()
{
    m_events &= ~kEpollEventWrite;
    update();
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}

void Channel::handle(Timestamp receiveTime)
{
    assert(m_receivedEvents != kEpollEventNone);

    if (m_receivedEvents & kEpollEventRead)
    {
        if (m_readCallback)
        {
            m_readCallback(receiveTime);
        }
    }

    if (m_receivedEvents & kEpollEventWrite)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }

    if (m_receivedEvents & kEpollEventError)
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }
}
