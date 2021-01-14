#include "Channel.h"

#include <assert.h>
#include <sys/epoll.h>

#include "EventLoop.h"
#include "Poller.h"

static const int kEventsNone = 0;
static const int kEventsRead = EPOLLIN | EPOLLRDHUP | EPOLLPRI;
static const int kEventsWrite = EPOLLOUT;
static const int kEventsError = EPOLLERR;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(kEventsNone)
    , m_receivedEvents(kEventsNone)
    , m_status(ChannelStatus::NEW)
{
}

void Channel::handle(Timestamp receivedTime)
{
    m_pLoop->assertLoopInThread();

    if (m_receivedEvents & kEventsError)
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_receivedEvents & kEventsRead)
    {
        if (m_readCallback)
        {
            m_readCallback(receivedTime);
        }
    }

    if (m_receivedEvents & kEventsWrite)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }
}

void Channel::enableReading()
{
    m_events |= kEventsRead;
    update();
}

void Channel::enableWriting()
{
    m_events |= kEventsWrite;
    update();
}

void Channel::disableWriting()
{
    m_events &= ~kEventsWrite;
    update();
}

void Channel::disableAll()
{
    m_events = kEventsNone;
    update();
}

bool Channel::hasEvents()
{
    return m_events != kEventsNone;
}

void Channel::update()
{
    m_pLoop->update(this);
}
