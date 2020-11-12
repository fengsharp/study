#include "Channel.h"

#include <poll.h>
#include <sstream>

#include "EventLoop.h"
#include "Logging.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop * loop, int fd)
    : m_loop(loop)
    , m_fd(fd)
    , m_events(0)
    , m_receiveEvents(0)
    , m_pollerIndex(-1)
    , m_bLogHup(true)
    , m_bTied(false)
    , m_bEventHandling(false)
    , m_bAddedToLoop(false)
{
}

Channel::~Channel()
{
    assert(!m_bEventHandling);
    // assert(!m_bAddedToLoop);
    // if (m_loop->isInLoopThread())
    // {
    //     assert(!m_loop->hasChannel(this));
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
    m_loop->updateChannel(this);
}

void Channel::remove()
{
    assert(isNoneEvents());
    m_bAddedToLoop = false;
    m_loop->removeChannel(this);
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
    m_bEventHandling = true;

    LOG_TRACE << receiveEventsToString();
    if ((m_receiveEvents & POLLHUP) && !(m_receiveEvents & POLLIN))
    {
        if (m_bLogHup)
        {
            LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLHUP";
        }
        if (m_closeCallback)
            m_closeCallback();
    }

    if (m_receiveEvents & POLLNVAL)
    {
        LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLNVAL";
    }

    if (m_receiveEvents & (POLLERR | POLLNVAL))
    {
        if (m_errorCallback)
            m_errorCallback();
    }
    if (m_receiveEvents & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (m_readCallback)
            m_readCallback(receiveTime);
    }
    if (m_receiveEvents & POLLOUT)
    {
        if (m_writeCallback)
            m_writeCallback();
    }

    m_bEventHandling = false;
}

string Channel::receiveEventsToString() const
{
    return eventsToString(m_fd, m_receiveEvents);
}

string Channel::eventsToString() const
{
    return eventsToString(m_fd, m_events);
}

string Channel::eventsToString(int fd, int ev)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLRDHUP)
        oss << "RDHUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";

    return oss.str();
}
