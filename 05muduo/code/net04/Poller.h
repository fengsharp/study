#ifndef POLLER_H_
#define POLLER_H_

#include <map>
#include <vector>

#include "Timestamp.h"
#include "EventLoop.h"

struct epoll_event;
class Channel;

class Poller : private NonCopyable
{
public:
    explicit Poller(EventLoop * pLoop);
    ~Poller();

    Timestamp poll(int timeoutMs, std::vector<Channel*> * pOutActiveChannels);
    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);
    bool hasChannel(Channel * pChannel);
    void assertInLoopThread() const
    {
        return m_pLoop->assertLoopInThisThread();
    }

private:
    void fillActiveChannels(int numEvents, std::vector<Channel*> * pOutActiveChannels);
    void update(int operation, Channel * pChannel);

private:
    EventLoop * m_pLoop;
    std::map<int, Channel *> m_mapChanels;

    int m_epollFd;
    std::vector<struct epoll_event> m_vecEvent;
    static const int kInitEventListSize = 16;
};

#endif