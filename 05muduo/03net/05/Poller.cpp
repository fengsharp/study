#include "Poller.h"

#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Types.h"

static const int kInitReceiveEventsCnt = 16;

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollfd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecTmpActivityEvents(kInitReceiveEventsCnt)
{
    assert(m_epollfd >= 0);
}

Poller::~Poller()
{
    ::close(m_epollfd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel*> * pOutActivityChannels)
{
    m_pLoop->assertLoopInThread();
    
    int nReady = epoll_wait(m_epollfd, m_vecTmpActivityEvents.data(), m_vecTmpActivityEvents.size(), timeoutMs);

    Timestamp now(Timestamp::now());
    
    for (int i = 0; i < nReady; i++)
    {
        struct epoll_event & item = m_vecTmpActivityEvents.at(i);
        Channel * pChannel = static_cast<Channel*>(item.data.ptr);
        pChannel->setReceiveEvents(item.events);
        pOutActivityChannels->push_back(pChannel);
    }
    
    if (nReady == m_vecTmpActivityEvents.size())
    {
        m_vecTmpActivityEvents.resize(m_vecTmpActivityEvents.size() * 2);
    }

    return now;
}

void Poller::updateChannel(Channel * pChanel)
{
    m_pLoop->assertLoopInThread();
    
    ChannelPollerStatus status = pChanel->polllerStatus();
    if (status == ChannelPollerStatus::NEW || status == ChannelPollerStatus::DELETED)
    {
        updateChannel(EPOLL_CTL_ADD, pChanel);
        pChanel->setPollerStatus(ChannelPollerStatus::ADDED);
    }
    else
    {
        updateChannel(EPOLL_CTL_MOD, pChanel);
    }
}


void Poller::updateChannel(int op, Channel * pChannel)
{
    int ret = 0;
    struct epoll_event epollEvent;
    memZero(&epollEvent, sizeof(epollEvent));
    epollEvent.events = pChannel->events();
    epollEvent.data.ptr = pChannel;

    if (pChannel->hasNoneEvents() && pChannel->polllerStatus() != ChannelPollerStatus::DELETED)
    {
        ret = epoll_ctl(m_epollfd, EPOLL_CTL_DEL, pChannel->fd(), &epollEvent);
        pChannel->setPollerStatus(ChannelPollerStatus::DELETED);
    }
    else
    {
        ret = epoll_ctl(m_epollfd, op, pChannel->fd(), &epollEvent);
    }
    assert(ret == 0);
}

void Poller::removeChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInThread();
    
    pChannel->clearAllEvents();
    updateChannel(pChannel);
}






















