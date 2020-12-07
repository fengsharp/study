#include "Poller.h"

#include <assert.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <sstream>

#include "Channel.h"
#include "Logging.h"
#include "Types.h"

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollFd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecEvents(kInitEventListSize)
{
}

Poller::~Poller()
{
    ::close(m_epollFd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel *> * pOutActiveChannels)
{
    int numEvents = ::epoll_wait(m_epollFd, m_vecEvents.data(), m_vecEvents.size(), timeoutMs);
    int savedErron = errno;
    Timestamp now(Timestamp::now());
    if (numEvents > 0)
    {
        fillActiveChannels(numEvents, pOutActiveChannels);
        if (implicit_cast<size_t>(numEvents) == m_vecEvents.size())
        {
            m_vecEvents.resize(m_vecEvents.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
        LOG_TRACE << "nothing happened";
    }
    else
    {
        if (savedErron != EINTR)
        {
            errno = savedErron;
            LOG_SYSERR << "poll()";
        }
    }
    return now;
}

void Poller::updateChannel(Channel * pChannel)
{
    assertInLoopThread();
    const PollerState state = pChannel->pollerState();

    if (state == PollerState::NEW || state == PollerState::DELETED)
    {
        int fd = pChannel->fd();
        if (state == PollerState::NEW)
        {
            assert(m_mapChannels.find(fd) == m_mapChannels.end());
            m_mapChannels[fd] = pChannel;
        }
        else
        {
            assert(m_mapChannels.find(fd) != m_mapChannels.end());
            assert(m_mapChannels[fd] == pChannel);
        }
        pChannel->setPollerState(PollerState::ADDED);
        update(EPOLL_CTL_ADD, pChannel);
    }
    else
    {
        int fd = pChannel->fd();
        (void)fd;
        assert(m_mapChannels.find(fd) != m_mapChannels.end());
        assert(m_mapChannels[fd] == pChannel);
        assert(state == PollerState::ADDED);

        if (pChannel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, pChannel);
            pChannel->setPollerState(PollerState::DELETED);
        }
        else
        {
            update(EPOLL_CTL_MOD, pChannel);
        }
    }
}

void Poller::removeChannel(Channel * pChannel)
{
    assertInLoopThread();
    int fd = pChannel->fd();
    assert(m_mapChannels.find(fd) != m_mapChannels.end());
    assert(m_mapChannels[fd] == pChannel);
    assert(pChannel->isNoneEvent());
    const PollerState state = pChannel->pollerState();
    assert(state == PollerState::ADDED || state == PollerState::DELETED);
    size_t n = m_mapChannels.erase(fd);
    (void)n;
    assert(n == 1);

    if (state == PollerState::ADDED)
    {
        update(EPOLL_CTL_DEL, pChannel);
    }
    pChannel->setPollerState(PollerState::NEW);
}

bool Poller::hasChannel(Channel * pChannel) const
{
    assertInLoopThread();
    std::map<int, Channel *>::const_iterator it = m_mapChannels.find(pChannel->fd());
    return it != m_mapChannels.end() && it->second == pChannel;
}

void Poller::fillActiveChannels(int numEvents, std::vector<Channel *> * pOutActiveChannels) const
{
    assert(implicit_cast<size_t>(numEvents) <= m_vecEvents.size());

    for (int i = 0; i < numEvents; ++i)
    {
        Channel * pChannel = static_cast<Channel *>(m_vecEvents[i].data.ptr);
#ifndef NDEBUG
        int fd = pChannel->fd();
        std::map<int, Channel *>::const_iterator it = m_mapChannels.find(fd);
        assert(it != m_mapChannels.end() && it->second == pChannel);
#endif
        pChannel->setReceiveEvents(m_vecEvents[i].events);
        pOutActiveChannels->push_back(pChannel);
    }
}

const char * operationToString(int op)
{
    switch (op)
    {
        case EPOLL_CTL_ADD:
            return "ADD";
        case EPOLL_CTL_DEL:
            return "DEL";
        case EPOLL_CTL_MOD:
            return "MOD";
        default:
            assert(false && "ERROR OP");
            return "Unknown Operation";
    }
}

std::string eventsToString(int fd, int env)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (env & EPOLLIN)
    {
        oss << "IN ";
    }
    if (env & EPOLLPRI)
    {
        oss << "RPI ";
    }
    if (env & EPOLLOUT)
    {
        oss << "OUT ";
    }
    if (env & EPOLLHUP)
    {
        oss << "HUP ";
    }
    if (env & EPOLLERR)
    {
        oss << "ERR ";
    }

    return oss.str();
}

void Poller::update(int operation, Channel * pChannel)
{
    struct epoll_event event;
    memZero(&event, sizeof(event));
    event.events = pChannel->events();
    event.data.ptr = pChannel;
    int fd = pChannel->fd();
    LOG_TRACE << "epoll_ctrl op=" << operationToString(operation)
              << " fd=" << fd << " event={" << eventsToString(fd, pChannel->events()) << "}";
    if (::epoll_ctl(m_epollFd, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_SYSERR << "epoll_ctl op = " << operationToString(operation) << " fd = " << fd;
        }
        else
        {
            LOG_SYSFATAL << "epoll_ctl op = " << operationToString(operation) << " fd = " << fd;
        }
    }
}