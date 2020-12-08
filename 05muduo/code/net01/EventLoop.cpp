#include "EventLoop.h"

#include <assert.h>
#include <pthread.h>

#include "Channel.h"
#include "Logging.h"
#include "Poller.h"

__thread EventLoop * t_loopInThisThread = NULL;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_bEventHanding(false)
    , m_threadId(CurrentThread::tid())
    , m_pPoller(std::make_unique<Poller>(this))
    , m_pCurrentActiveChannel(NULL)
{
    assert(t_loopInThisThread == NULL);
    t_loopInThisThread = this;
}

EventLoop::~EventLoop()
{
    assert(m_bLooping == false);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(m_bLooping == false);
    m_bLooping = true;
    m_bQuit = false;

    while (m_bQuit == false)
    {
        m_vecActiveChannels.clear();
        m_pollReturnTime = m_pPoller->poll(kPollTimeMs, &m_vecActiveChannels);

        m_bEventHanding = true;
        for (auto & item : m_vecActiveChannels)
        {
            m_pCurrentActiveChannel = item;
            item->handleEvent(m_pollReturnTime);
        }
        m_pCurrentActiveChannel = NULL;
        m_bEventHanding = false;
    }

    m_bLooping = false;
}

void EventLoop::quit()
{
    m_bQuit = true;
    if (isInLoopThread() == false)
    {
        // wakeup
    }
}

bool EventLoop::hasChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertLoopInThisThread();
    return m_pPoller->hasChannel(pChannel);
}

void EventLoop::removeChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertLoopInThisThread();
    m_pPoller->removeChannel(pChannel);
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertLoopInThisThread();
    if (m_bEventHanding)
    {
        assert(m_pCurrentActiveChannel == pChannel || std::find(m_vecActiveChannels.begin(), m_vecActiveChannels.end(), pChannel) == m_vecActiveChannels.end());
    }
    m_pPoller->updateChannel(pChannel);
}