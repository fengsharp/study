#include "Poller.h"

#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Types.h"

static const int POLLER_INIT_CNT = 16;

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollfd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecWaitEvnets(POLLER_INIT_CNT)
{
}

Poller::~Poller()
{
    ::close(m_epollfd);
}

void Poller::updateChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInThread();

    ChannelStatus status = pChannel->status();
    if (status == ChannelStatus::NEW || status == ChannelStatus::DELETED)
    {
        update(EPOLL_CTL_ADD, pChannel);
        pChannel->setStatus(ChannelStatus::ADDED);
    }
    else
    {
        update(EPOLL_CTL_MOD, pChannel);
    }
}

void Poller::removeChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInThread();

    if (pChannel->status() != ChannelStatus::ADDED)
    {
        return;
    }

    pChannel->disableAll();
    updateChannel(pChannel);
}

void Poller::update(int op, Channel * pChannel)
{
    struct epoll_event epollEvents;
    memZero(&epollEvents, sizeof(epollEvents));
    epollEvents.events = pChannel->events();
    epollEvents.data.ptr = pChannel;

    if (pChannel->hasEvents())
    {
        int ret = epoll_ctl(m_epollfd, op, pChannel->fd(), &epollEvents);
        assert(ret == 0);
    }
    else
    {
        int ret = epoll_ctl(m_epollfd, EPOLL_CTL_DEL, pChannel->fd(), &epollEvents);
        assert(ret == 0);
        pChannel->setStatus(ChannelStatus::DELETED);
    }
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel*> * pOutActivityChannels)
{
    int nReady = epoll_wait(m_epollfd, m_vecWaitEvnets.data(), m_vecWaitEvnets.size(), timeoutMs);

    Timestamp now(Timestamp::now());

    for (int i = 0; i < nReady; ++i)
    {
        struct epoll_event & event = m_vecWaitEvnets[i];
        Channel * pChannel = static_cast<Channel *>(event.data.ptr);

        pChannel->setReceivedEvents(event.events);
        pOutActivityChannels->push_back(pChannel);
    }

    if (nReady == m_vecWaitEvnets.size())
    {
        m_vecWaitEvnets.resize(m_vecWaitEvnets.size() * 2);
    }

    return now;
}







