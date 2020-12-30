#include "Poller.h"

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "Types.h"

static_assert(EPOLLIN == POLLIN, "EPOLLIN == POLLIN");
static_assert(EPOLLPRI == POLLPRI, "EPOLLPRI == POLLPRI");
static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT == POLLOUT");
static_assert(EPOLLRDHUP == POLLRDHUP, "EPOLLRDHUP == POLLRDHUP");
static_assert(EPOLLERR == POLLERR, "EPOLLERR == POLLERR");
static_assert(EPOLLHUP == POLLHUP, "EPOLLHUP == POLLHUP");

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

static const int kInitEventListsize = 16;

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
    int numEvents = ::epoll_wait(m_epollFd, m_vecEvents.data(), m_vecEvents.size(), timeoutMs);
    int savedErron = errno;
    Timestamp now(Timestamp::now());

    if (numEvents > 0)
    {
        fillActiveChannels(numEvents, pOutChannels);
        if (implicit_cast<size_t>(numEvents) == m_vecEvents.size())
        {
            m_vecEvents.resize(m_vecEvents.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
    }
    else
    {
        if (savedErron != EINTR)
        {
            errno = savedErron;
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
    const int state = pChannel->status();
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
        pChannel->setStatus(kAdded);
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
            pChannel->setStatus(kDeleted);
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
    int state = pChannel->status();
    assert(state == kAdded || state == kDeleted);
    size_t n = m_mapChannels.erase(fd);
    (void)n;
    assert(n == 1);

    if (state == kAdded)
    {
        update(EPOLL_CTL_DEL, pChannel);
    }
    pChannel->setStatus(kNew);
}

void Poller::update(int operation, Channel * pChannel)
{
    struct epoll_event event;
    memZero(&event, sizeof(event));
    event.events = pChannel->events();
    event.data.ptr = pChannel;
    int fd = pChannel->fd();

    if (::epoll_ctl(m_epollFd, operation, fd, &event) < 0)
    {
        abort();
    }
}

bool Poller::hasChannel(Channel * pChannel) const
{
    assertInLoopThread();
    std::map<int, Channel *>::const_iterator it = m_mapChannels.find(pChannel->fd());
    return it != m_mapChannels.end() && it->second == pChannel;
}
