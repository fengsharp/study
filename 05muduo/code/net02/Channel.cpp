#include "Channel.h"

#include <assert.h>

#include <sstream>
#include <sys/epoll.h>

#include "EventLoop.h"
#include "Logging.h"

const int Channel::kNoneEvnet = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_evnets(kNoneEvnet)
    , m_receiveEvents(kNoneEvnet)
    , m_ctlStatus(CtlState::NEW)
    , m_bEventHanding(false)
    , m_bAddedToLoop(false)
    , m_readCallback(NULL)
    , m_writeCallback(NULL)
    , m_errorCallback(NULL)
{
}

Channel::~Channel()
{
    assert(m_bEventHanding == false);
}

void Channel::update()
{
    m_bAddedToLoop = true;
    m_pLoop->updateChannel(this);
}

void Channel::remove()
{
    assert(isNoneEvents());
    m_bAddedToLoop = false;
    m_pLoop->removeChannel(this);   
}

void Channel::handleEvent(Timestamp receiveTime)
{
    m_bEventHanding = true;

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

    m_bEventHanding = false;
}



