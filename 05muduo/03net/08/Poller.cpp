#include "Poller.h"

#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Channel.h"
#include "Types.h"
#include "EventLoop.h"

static const int POLLER_INIT_FD_LIST_SIZE = 16;

Poller::Poller(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_epollfd(::epoll_create1(EPOLL_CLOEXEC))
    , m_vecPollfdList(POLLER_INIT_FD_LIST_SIZE)
{
}

Poller::~Poller()
{
    ::close(m_epollfd);
}

Timestamp Poller::poll(int timeoutMs, std::vector<Channel *> * pOutActiveChannels)
{
    m_pLoop->assertLoopInThread();

    int nRead = epoll_wait(m_epollfd, m_vecPollfdList.data(), m_vecPollfdList.size(), timeoutMs);

    Timestamp now(Timestamp::now());

    for (int i = 0; i < nRead; i++)
    {
        struct epoll_event & activeEvents = m_vecPollfdList.at(i);
        Channel * pChannel = static_cast<Channel *>(activeEvents.data.ptr);
        pChannel->setReceivedEvents(activeEvents.events);
        pOutActiveChannels->push_back(pChannel);
    }

    if (nRead == m_vecPollfdList.size())
    {
        m_vecPollfdList.resize(m_vecPollfdList.size() * 2);
    }

    return now;
}

void Poller::updateChannel(Channel * pChannel)
{
    m_pLoop->assertLoopInThread();
    
    PollerStatus status = pChannel->status();

    if (status == PollerStatus::ADDED)
    {
        update(EPOLL_CTL_MOD, pChannel);
    }
    else if (status == PollerStatus::NEW || status == PollerStatus::DELETED)
    {
        update(EPOLL_CTL_ADD, pChannel);
        pChannel->setStatus(PollerStatus::ADDED);
    }
    else
    {
        abort();
    }
}

void Poller::update(int op, Channel * pChannel)
{
    struct epoll_event event;
    memZero(&event, sizeof(event));
    event.events = pChannel->events();
    event.data.ptr = pChannel;
    int ret = epoll_ctl(m_epollfd, op, pChannel->fd(), &event);
    assert(ret == 0);
}
