#include "Poller.h"

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "Logging.h"

// On Linux, the constants of poll(2) and epoll(4)
// are expected to be the same.
static_assert(EPOLLIN == POLLIN, "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI, "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT, "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP, "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR, "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP, "epoll uses same flag values as poll");

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

Poller::Poller(EventLoop * loop)
    : m_ownerLoop(loop)
    , m_epollId(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecEvents(kInitEventListSize)
{
    if (m_epollId < 0)
    {
        LOG_SYSFATAL << "EPollPoller::EPollPoller";
    }
}

Poller::~Poller()
{
    ::close(m_epollId);
}

Timestamp Poller::poll(int timeoutMs, ChannelList * activeChannels)
{
    LOG_TRACE << "fd total count " << m_mapChannel.size();
    int numEvents = ::epoll_wait(m_epollId,
                                 m_vecEvents.data(),
                                 static_cast<int>(m_vecEvents.size()),
                                 timeoutMs);
    int savedErrno = errno;
    Timestamp now(Timestamp::now());
    if (numEvents > 0)
    {
        LOG_TRACE << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
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
        // error happens, log uncommon ones
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_SYSERR << "EPollPoller::poll()";
        }
    }
    return now;
}

/*
typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;    // Epoll events
    epoll_data_t data;      // User data variable
};
*/
void Poller::fillActiveChannels(int numEvents, ChannelList * activeChannels) const
{
    assert(implicit_cast<size_t>(numEvents) <= m_vecEvents.size());

    for (int i = 0; i < numEvents; ++i)
    {
        Channel * channel = static_cast<Channel *>(m_vecEvents[i].data.ptr);
#ifndef NDEBUG
        int fd = channel->fd();
        std::map<int, Channel *>::const_iterator it = m_mapChannel.find(fd);
        assert(it != m_mapChannel.end());
        assert(it->second == channel);
#endif
        channel->setReceiveEvents(m_vecEvents[i].events);
        activeChannels->push_back(channel);
    }
}

void Poller::updateChannel(Channel * pChannel)
{
    assertInLoopThread();
    const int index = pChannel->pollerIndex();
    LOG_TRACE << "fd = " << pChannel->fd()
              << " events = " << pChannel->events() << " index = " << index;
    if (index == kNew || index == kDeleted)
    {
        // a new one, add with EPOLL_CTL_ADD
        int fd = pChannel->fd();
        if (index == kNew)
        {
            assert(m_mapChannel.find(fd) == m_mapChannel.end());
            m_mapChannel[fd] = pChannel;
        }
        else // index == kDeleted
        {
            assert(m_mapChannel.find(fd) != m_mapChannel.end());
            assert(m_mapChannel[fd] == pChannel);
        }

        pChannel->setPollerIndex(kAdded);
        update(EPOLL_CTL_ADD, pChannel);
    }
    else
    {
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = pChannel->fd();
        (void)fd;
        assert(m_mapChannel.find(fd) != m_mapChannel.end());
        assert(m_mapChannel[fd] == pChannel);
        assert(index == kAdded);
        if (pChannel->isNoneEvents())
        {
            update(EPOLL_CTL_DEL, pChannel);
            pChannel->setPollerIndex(kDeleted);
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
    LOG_TRACE << "fd = " << fd;
    assert(m_mapChannel.find(fd) != m_mapChannel.end());
    assert(m_mapChannel[fd] == pChannel);
    assert(pChannel->isNoneEvents());
    int index = pChannel->pollerIndex();
    assert(index == kAdded || index == kDeleted);
    size_t n = m_mapChannel.erase(fd);
    (void)n;
    assert(n == 1);

    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, pChannel);
    }
    pChannel->setPollerIndex(kNew);
}

void Poller::update(int operation, Channel * pChannel)
{
    struct epoll_event event;
    memZero(&event, sizeof event);
    event.events = pChannel->events();
    event.data.ptr = pChannel;
    int fd = pChannel->fd();
    LOG_TRACE << "epoll_ctl op = " << operationToString(operation)
              << " fd = " << fd << " event = { " << pChannel->eventsToString() << " }";
    if (::epoll_ctl(m_epollId, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_SYSERR << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
        else
        {
            LOG_SYSFATAL << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
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
            assert(false && "ERROR op");
            return "Unknown Operation";
    }
}

bool Poller::hasChannel(Channel * pChannel) const
{
    assertInLoopThread();
    std::map<int, Channel *>::const_iterator it = m_mapChannel.find(pChannel->fd());
    return it != m_mapChannel.end() && it->second == pChannel;
}