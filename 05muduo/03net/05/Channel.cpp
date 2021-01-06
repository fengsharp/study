#include "Channel.h"

#include <assert.h>
#include <sys/epoll.h>

#include "EventLoop.h"
#include "Poller.h"
#include "Types.h"

static const int kEpollEventNone = 0;
static const int kEpollEventRead = EPOLLIN | EPOLLRDHUP | EPOLLPRI;
static const int kEpollEventWrite = EPOLLOUT;
static const int kEpollEventError = EPOLLERR;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(kEpollEventNone)
    , m_receivedEvents(kEpollEventNone)
    , m_pollerStatus(ChannelPollerStatus::NEW)
{
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

void Channel::disableWriting()
{
    m_events &= ~kEpollEventWrite;
    update();
}

void Channel::disableAll()
{
    clearAllEvents();
    update();
}

void Channel::clearAllEvents()
{
    m_events = kEpollEventNone;
}

bool Channel::hasNoneEvents() const
{
    return m_events == kEpollEventNone;
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}

void Channel::handleEvent(Timestamp receivedTime)
{
    if (m_events & kEpollEventError)
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_events & kEpollEventWrite)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }

    if (m_events & kEpollEventRead)
    {
        if (m_readCallback)
        {
            m_readCallback(receivedTime);
        }
    }
}