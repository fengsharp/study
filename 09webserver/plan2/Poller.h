#ifndef POLLER_H_
#define POLLER_H_

#include <map>
#include <vector>

#include "EventLoop.h"
#include "NoneCopyable.h"
#include "Timestamp.h"

struct epoll_event;
class Channel;

class Poller : private NoneCopyable
{
public:
    typedef std::vector<Channel *> ChannelList;

    Poller(EventLoop * loop);
    ~Poller();

    Timestamp poll(int timeoutMs, ChannelList * activeChannels);
    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);

    void assertInLoopThread() const
    {
        m_ownerLoop->assertInLoopThread();
    }

    bool hasChannel(Channel* pChannel) const;
private:
    static const int kInitEventListSize = 16;
    static const char * operationToString(int op);

    void fillActiveChannels(int numEvents, ChannelList * activeChannels) const;
    void update(int operation, Channel * pChannel);

private:
    EventLoop * m_ownerLoop;
    std::map<int, Channel*> m_mapChannel;
    int m_epollId;
    std::vector<struct epoll_event> m_vecEvents;
};

#endif