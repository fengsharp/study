#include "Poller.h"

#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "Logging.h"

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollFd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecEvent(kInitEventListSize)
{
}

Poller::~Poller()
{
    ::close(m_epollFd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel *> * pOutActiveChannels)
{
    int numEvents = ::epoll_wait(m_epollFd, m_vecEvent.data(), m_vecEvent.size(), timeoutMs);
    Timestamp now(Timestamp::now());
    if (numEvents > 0)
    {
        fillActiveChannels(numEvents, pOutActiveChannels);
        if (implicit_cast<size_t>(numEvents) == m_vecEvent.size())
        {
            m_vecEvent.resize(m_vecEvent.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
        LOG_TRACE << "nothing happend.";
    }
    else
    {
        if (errno != EINTR)
        {
            LOG_SYSERR << "poll()";
        }
    }

    return now;
}
void Poller::fillActiveChannels(int numEvents, std::vector<Channel *> * pOutActiveChannels)
{
    assert(implicit_cast<size_t>(numEvents) <= m_vecEvent.size());

    for (const struct epoll_event & item : m_vecEvent)
    {
        Channel * pChannel = static_cast<Channel *>(item.data.ptr);

#ifndef NDEBUG
        int fd = pChannel->fd();
        std::map<int, Channel *>::const_iterator it = m_mapChanels.find(fd);
        assert(it != m_mapChanels.end() && it->second == pChannel);
#endif

        pChannel->setReceiveEvents(item.events);
        pOutActiveChannels->push_back(pChannel);
    }
}

void Poller::updateChannel(Channel * pChannel)
{
    assertInLoopThread();
    const CtlState state = pChannel->ctlState();

    if (state == CtlState::NEW || state == CtlState::DELETED)
    {
        int fd = pChannel->fd();
        if (state == CtlState::NEW)
        {
            assert(m_mapChanels.find(fd) == m_mapChanels.end());
            m_mapChanels[fd] = pChannel;
        }
        else
        {
            assert(m_mapChanels.find(fd) != m_mapChanels.end());
            assert(m_mapChanels[fd] == pChannel);
        }
        pChannel->setCtlState(CtlState::ADDED);
        update(EPOLL_CTL_ADD, pChannel);
    }
    else // added
    {
        int fd = pChannel->fd();
        (void)fd;
        assert(m_mapChanels.find(fd) != m_mapChanels.end());
        assert(m_mapChanels[fd] == pChannel);
        assert(state == CtlState::ADDED);
        if (pChannel->isNoneEvents())
        {
            update(EPOLL_CTL_DEL, pChannel);
            pChannel->setCtlState(CtlState::DELETED);
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
    assert(m_mapChanels.find(fd) != m_mapChanels.end());
    assert(m_mapChanels[fd] == pChannel);
    assert(pChannel->isNoneEvents());
    CtlState state = pChannel->ctlState();
    assert(state == CtlState::ADDED || state == CtlState::DELETED);
    size_t n = m_mapChanels.erase(fd);
    (void)n;
    assert(n == 1);
    if (state == CtlState::ADDED)
    {
        update(EPOLL_CTL_DEL, pChannel);
    }
    pChannel->setCtlState(CtlState::NEW);
}

bool Poller::hasChannel(Channel * pChannel)
{
    assertInLoopThread();
    std::map<int, Channel*>::const_iterator it = m_mapChanels.find(pChannel->fd());
    return it != m_mapChanels.end() && it->second == pChannel;
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
        LOG_SYSERR << "epoll_ctl";
    }
}