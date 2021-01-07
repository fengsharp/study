#include "Poller.h"

#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Types.h"

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollfd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecTmpActivityEvnets(16)
{
    assert(m_epollfd >= 0);
}

Poller::~Poller()
{
    ::close(m_epollfd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel *> * pOutActivityChannels)
{
    // m_pLoop->assertLoopInthread();

    int nReady = epoll_wait(m_epollfd, m_vecTmpActivityEvnets.data(), m_vecTmpActivityEvnets.size(), timeoutMs);
    assert(nReady >= 0);
    Timestamp tmNow(Timestamp::now());

    for (int i = 0; i < nReady; i++)
    {
        struct epoll_event & itemEvnet = m_vecTmpActivityEvnets.at(i);
        Channel * pChannel = static_cast<Channel *>(itemEvnet.data.ptr);
        pChannel->setReceivedEvents(itemEvnet.events);
        pOutActivityChannels->push_back(pChannel);
    }

    if (nReady == m_vecTmpActivityEvnets.size())
    {
        m_vecTmpActivityEvnets.resize(m_vecTmpActivityEvnets.size() * 2);
    }

    return tmNow;
}

void Poller::updateChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInthread();

    ChannelPollerStatus status = pChannel->pollerStatus();

    if (status == ChannelPollerStatus::NEW || status == ChannelPollerStatus::DELETED)
    {
        updateChannel(EPOLL_CTL_ADD, pChannel);
        pChannel->setPollerStatus(ChannelPollerStatus::ADDED);
    }
    else
    {
        updateChannel(EPOLL_CTL_MOD, pChannel);
    }
}

void Poller::removeChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInthread();
    if (pChannel->pollerStatus() != ChannelPollerStatus::DELETED)
    {
        pChannel->clearAllEvents();
        updateChannel(EPOLL_CTL_DEL, pChannel);
        pChannel->setPollerStatus(ChannelPollerStatus::DELETED);
    }
}

void Poller::updateChannel(int op, Channel * pChannel)
{
    struct epoll_event epollEvent;
    memZero(&epollEvent, sizeof(epollEvent));
    epollEvent.events = pChannel->events();
    epollEvent.data.ptr = pChannel;

    int ret = epoll_ctl(m_epollfd, op, pChannel->fd(), &epollEvent);
    assert(ret == 0);
}