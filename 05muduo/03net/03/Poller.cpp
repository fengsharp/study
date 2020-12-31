#include "Poller.h"

#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Types.h"

const static int kInitReceiveSize = 16;

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollfd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecTmpReceiveEvents(kInitReceiveSize)
{
}

Poller::~Poller()
{
    ::close(m_epollfd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel *> * pOutActiveChanels)
{
    assertInLoopThread();
    assert(m_vecTmpReceiveEvents.size() >= kInitReceiveSize);

    Timestamp receiveTime = Timestamp::now();
    int nready = epoll_wait(m_epollfd, m_vecTmpReceiveEvents.data(), static_cast<int>(m_vecTmpReceiveEvents.size()), -1);
    if (nready < 0)
    {
        abort();
    }

    assert(implicit_cast<size_t>(nready) <= m_vecTmpReceiveEvents.size());
    for (int i = 0; i < nready; i++)
    {
        Channel * pChannel = static_cast<Channel *>(m_vecTmpReceiveEvents[i].data.ptr);
        pChannel->setReceiveEvents(m_vecTmpReceiveEvents[i].events);
        pOutActiveChanels->push_back(pChannel);
    }

    if (implicit_cast<size_t>(nready) == m_vecTmpReceiveEvents.size())
    {
        m_vecTmpReceiveEvents.resize(m_vecTmpReceiveEvents.size() * 2);
    }

    return receiveTime;
}

void Poller::updateChannel(Channel * pChannel)
{
    assertInLoopThread();
    
    if (pChannel->pollerSate() == PollerState::NEW || pChannel->pollerSate() == PollerState::DELETED)
    {

        pChannel->setPollerState(PollerState::ADDED);
        update(EPOLL_CTL_ADD, pChannel);
    }
    else if (pChannel->pollerSate() == PollerState::ADDED)
    {
        if (pChannel->isNoneEvents())
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
    assert(pChannel->pollerSate() == PollerState::ADDED || pChannel->pollerSate() == PollerState::DELETED);
    if (pChannel->pollerSate() == PollerState::ADDED)
    {
        update(EPOLL_CTL_DEL, pChannel);
    }
    pChannel->setPollerState(PollerState::NEW);
}

void Poller::update(int operation, Channel * pChannel)
{
    struct epoll_event event;
    memZero(&event, sizeof(event));
    event.events = pChannel->events();
    event.data.ptr = pChannel;
    
    int ret = epoll_ctl(m_epollfd, operation, pChannel->fd(), &event);
    assert(ret == 0);
}

void Poller::assertInLoopThread()
{
    m_pLoop->assertInLoopThread();
}