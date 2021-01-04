#include "Poller.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Types.h"

/*
int epoll_create1(int flags); EPOLL_CLOEXEC
int epoll_wait(int epfd, struct epoll_event *events,
                      int maxevents, int timeout);

typedef union epoll_data {
               void    *ptr;
               int      fd;
               uint32_t u32;
               uint64_t u64;
           } epoll_data_t;

struct epoll_event {
    uint32_t     events;
    epoll_data_t data;
};
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

EPOLL_CTL_ADD EPOLL_CTL_MOD EPOLL_CTL_DEL
*/

static const int INIT_POLL_SIZE = 16;

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollid(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecTmpContainer(INIT_POLL_SIZE)
{
    assert(m_epollid >= 0);
}

Poller::~Poller()
{
    ::close(m_epollid);
}

Timestamp Poller::poll(int waitMs, std::vector<Channel *> * pOutActiveChannels)
{
    m_pLoop->assertLoopInThread();

    Timestamp now(Timestamp::now());
    int nReady = ::epoll_wait(m_epollid, m_vecTmpContainer.data(), m_vecTmpContainer.size(), waitMs);
    if (nReady < 0)
    {
        abort();
    }

    for (int i = 0; i < nReady; i++)
    {
        struct epoll_event & epollEvent = m_vecTmpContainer[i];
        Channel * pChannel = static_cast<Channel *>(epollEvent.data.ptr);
        pOutActiveChannels->push_back(pChannel);
        pChannel->setReciveEvents(epollEvent.events);
    }

    if (nReady == m_vecTmpContainer.size())
    {
        m_vecTmpContainer.resize(m_vecTmpContainer.size() * 2);
    }
    
    return now;
}

void Poller::updateChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInThread();

    PollerStatus status = pChannel->pollerStatus();
    if (status == PollerStatus::NEW || status == PollerStatus::DELETED)
    {
        updateChannel(EPOLL_CTL_ADD, pChannel);

        pChannel->setPollerStatus(PollerStatus::ADDED);
    }
    else
    {
        if (pChannel->hasNoneEvents())
        {
            updateChannel(EPOLL_CTL_DEL, pChannel);
            pChannel->setPollerStatus(PollerStatus::DELETED);
        }
        else
        {
            updateChannel(EPOLL_CTL_MOD, pChannel);
        }
    }
}

void Poller::removeChannel(Channel * pChannel)
{
    assert(pChannel->pollerStatus() != PollerStatus::DELETED);
    if (pChannel->pollerStatus() == PollerStatus::ADDED)
    {
        updateChannel(EPOLL_CTL_DEL, pChannel);
        pChannel->setPollerStatus(PollerStatus::DELETED);
    }
}

void Poller::updateChannel(int op, Channel * pChannel)
{
    struct epoll_event epollEvent;
    memZero(&epollEvent, sizeof(epollEvent));
    epollEvent.events = pChannel->events();
    epollEvent.data.ptr = pChannel;
    int ret = epoll_ctl(m_epollid, op, pChannel->fd(), &epollEvent);
    assert(ret == 0);
}