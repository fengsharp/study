#include "Channel.h"

#include <assert.h>

#include <sstream>

#include <sys/epoll.h>

#include "Logging.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(kNoneEvent)
    , m_receiveEvents(kNoneEvent)
    , m_pollerState(PollerState::NEW)
{
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}

// EPOLLIN EPOLLRDHUP EPOLLPRI      EPOLLHUP EPOLLET EPOLLONESHOT
// EPOLLOUT
// EPOLLERR
void Channel::handleEvent(Timestamp receiveTime)
{
    if (m_receiveEvents & EPOLLERR)
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

void Channel::remove()
{
    assert(isNoneEvent());

    // m_bAddedToLoop = false;
    m_pLoop->removeChannel(this);
}
