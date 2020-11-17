#include "Poller.h"

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "Logging.h"

static_assert(EPOLLIN == POLLIN, "EPOLLIN == POLLIN");
static_assert(EPOLLPRI == POLLPRI, "EPOLLPRI == POLLPRI");
static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT == POLLOUT");
static_assert(EPOLLRDHUP == POLLRDHUP, "EPOLLRDHUP == POLLRDHUP");
static_assert(EPOLLERR == POLLERR, "EPOLLERR == POLLERR");
static_assert(EPOLLHUP == POLLHUP, "EPOLLHUP == POLLHUP");

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollFd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecEvents(kInitEventListsize)
{
}

Poller::~Poller()
{
    ::close(m_epollFd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel *> * pOutChannels)
{
    LOG_TRACE << "fd total count" << m_mapChannels.size();
    int numEvents = ::epoll_wait(m_epollFd, m_vecEvents.data(), m_vecEvents.size(), timeoutMs);
    int savedErron = errno;
    Timestamp now(Timestamp::now());
    if (numEvents > 0)
    {
        LOG_TRACE << numEvents << " events happened";
        fillActiveChannels(numEvents, pOutChannels);
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

void Poller::fillActiveChannels(int numEvents, std::vector<Channel *> * pOutActiveChannles) const
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
        pOutActiveChannles->push_back(pChannel);
    }
}

void Poller::updateChannel(Channel * pChannel)
{
    assertInLoopThread();
    const int state = pChannel->pollerState();
    LOG_TRACE << "fd=" << pChannel->fd() << " events=" << pChannel->events() << " state=" << state;
    if (state == kNew || state == kDeleted)
    {
        int fd = pChannel->fd();
        if (state == kNew)
        {
            assert(m_mapChannels.find(fd) == m_mapChannels.end());
            m_mapChannels[fd] = pChannel;
        }
        else
        {
            assert(m_mapChannels.find(fd) != m_mapChannels.end());
            assert(m_mapChannels[fd] == pChannel);
        }
        pChannel->setPollerState(kAdded);
        update(EPOLL_CTL_ADD, pChannel);
    }
    else
    {
        int fd = pChannel->fd();
        (void)fd;
        assert(m_mapChannels.find(fd) != m_mapChannels.end());
        assert(m_mapChannels[fd] == pChannel);
        assert(state == kAdded);
        if (pChannel->isNoneEvents())
        {
            update(EPOLL_CTL_DEL, pChannel);
            pChannel->setPollerState(kDeleted);
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
    assert(pChannel->isNoneEvents());
    int state = pChannel->pollerState();
    assert(state == kAdded || state == kDeleted);
    size_t n = m_mapChannels.erase(fd);
    (void)n;
    assert(n == 1);

    if (state == kAdded)
    {
        update(EPOLL_CTL_DEL, pChannel);
    }
    pChannel->setPollerState(kNew);
}

void Poller::update(int operation, Channel * pChannel)
{
    struct epoll_event event;
    memZero(&event, sizeof(event));
    event.events = pChannel->events();
    event.data.ptr = pChannel;
    int fd = pChannel->fd();
    LOG_TRACE << "epoll_ctrl op=" << operationToString(operation)
              << " fd=" << fd << " event={" << pChannel->eventsToString() << "}";
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

const char * Poller::operationToString(int op)
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

bool Poller::hasChannel(Channel * pChannel) const
{
    assertInLoopThread();
    std::map<int, Channel *>::const_iterator it = m_mapChannels.find(pChannel->fd());
    return it != m_mapChannels.end() && it->second == pChannel;
}