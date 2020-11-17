#include "Channel.h"

#include <poll.h>
#include <sstream>

#include "EventLoop.h"
#include "Logging.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop * pLoop, int fd)
    : m_pLoop(pLoop)
    , m_fd(fd)
    , m_events(0)
    , m_revents(0)
    , m_pollerStatus(-1)
    , m_bLogHup(true)
    , m_bTied(false)
    , m_bEventHanding(false)
    , m_bAddedToLoop(false)
{
}

Channel::~Channel()
{
    assert(m_bEventHanding == false);
    // assert(m_bAddedToLoop == false);
    // if (m_pLoop->isInLoopThread())
    // {
    //     assert(m_pLoop->hasChannel(this) == false);
    // }
}

void Channel::tie(const std::shared_ptr<void> & obj)
{
    m_tie = obj;
    m_bTied = true;
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
    std::shared_ptr<void> guard;
    if (m_bTied)
    {
        guard = m_tie.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    m_bEventHanding = true;
    LOG_TRACE << reventsToString();

    if ((m_revents & POLLHUP) && !(m_revents & POLLIN))
    {
        if (m_bLogHup)
        {
            LOG_WARN << "fd=" << m_fd << " Channel::handle_event() POLLHUP";
        }
        if (m_closeCallback)
        {
            m_closeCallback();
        }
    }

    if (m_revents & POLLNVAL)
    {
        LOG_WARN << "fd=" << m_fd << " Channle::handle_event() POLLNVAL";
    }

    if (m_revents & (POLLERR | POLLNVAL))
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_revents & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (m_readCallback)
        {
            m_readCallback(receiveTime);
        }
    }

    if (m_revents & POLLOUT)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }

    m_bEventHanding = false;
}

string Channel::reventsToString() const
{
    return eventsToString(m_fd, m_revents);
}

string Channel::eventsToString() const
{
    return eventsToString(m_fd, m_events);
}

string Channel::eventsToString(int fd, int env)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (env & POLLIN)
    {
        oss << "IN ";
    }
    if (env & POLLPRI)
    {
        oss << "RPI ";
    }
    if (env & POLLOUT)
    {
        oss << "OUT ";
    }
    if (env & POLLHUP)
    {
        oss << "HUP ";
    }
    if (env & POLLERR)
    {
        oss << "ERR ";
    }
    if (env & POLLNVAL)
    {
        oss << "NVAL";
    }

    return oss.str();
}