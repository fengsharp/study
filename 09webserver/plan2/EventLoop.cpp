#include "EventLoop.h"

#include <assert.h>
#include <poll.h>
#include <stdio.h>

#include "Channel.h"
#include "Logging.h"
#include "Poller.h"

__thread EventLoop * t_loopInThisThread = nullptr;
const int kPollTimeMs = 10000;

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    : m_bLooping(false)
    , m_bQuit(false)
    , m_bEventHanding(false)
    , m_threadId(CurrentThread::gettid())
    , m_poller(new Poller(this))
    , m_pCurrentActiveChannel(nullptr)
{
    // printf("log level: %d\n", Logger::logLevel());

    LOG_TRACE << "create eventloop, thread id: " << m_threadId;
    if (t_loopInThisThread)
    {
        LOG_FATAL << "one eventloop per thread. fatal thread id: " << m_threadId;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    t_loopInThisThread = nullptr;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "eventloop created thread id: " << m_threadId
              << ". But now current thread id: " << CurrentThread::gettid()
              << ".";
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    // 断言当前处于创建该对象的线程中
    assertInLoopThread();
    m_bLooping = true;
    LOG_TRACE << "EventLoop " << this << " start looping";

    //::poll(NULL, 0, 5*1000);
    while (!m_bQuit)
    {
        m_vecActiveChannels.clear();
        m_pollReturnTime = m_poller->poll(kPollTimeMs, &m_vecActiveChannels);
        //++iteration_;
        if (Logger::logLevel() <= Logger::TRACE)
        {
            printActiveChannels();
        }
        // TODO sort channel by priority
        m_bEventHanding = true;

        for (std::vector<Channel *>::iterator it = m_vecActiveChannels.begin();
             it != m_vecActiveChannels.end(); ++it)
        {
            m_pCurrentActiveChannel = *it;
            m_pCurrentActiveChannel->handleEvent(m_pollReturnTime);
        }
        m_pCurrentActiveChannel = NULL;

        m_bEventHanding = false;
        //doPendingFunctors();
    }

    LOG_TRACE << "EventLoop " << this << " stop looping";
    m_bLooping = false;
}

bool EventLoop::hasChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    return m_poller->hasChannel(pChannel);
}

void EventLoop::printActiveChannels() const
{
    for (const Channel * pChannel : m_vecActiveChannels)
    {
        LOG_TRACE << "{" << pChannel->receiveEventsToString() << "} ";
    }
}

void EventLoop::quit()
{
    m_bQuit = true;
    if (!isInLoopThread())
    {
        // wakeup();
    }
}

void EventLoop::updateChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    m_poller->updateChannel(pChannel);
}

void EventLoop::removeChannel(Channel * pChannel)
{
    assert(pChannel->ownerLoop() == this);
    assertInLoopThread();
    if (m_bEventHanding)
    {
        assert(m_pCurrentActiveChannel == pChannel || std::find(m_vecActiveChannels.begin(), m_vecActiveChannels.end(), pChannel) == m_vecActiveChannels.end());
    }
    m_poller->removeChannel(pChannel);
}