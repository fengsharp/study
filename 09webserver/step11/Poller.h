#ifndef POLLER_H_
#define POLLER_H_

#include <map>
#include <vector>

#include "Timestamp.h"
#include "EventLoop.h"

struct epoll_event;
class Channel;

class Poller : private NoneCopyable
{
public:
    explicit Poller(EventLoop* pLoop);
    ~Poller();

    Timestamp poll(int timeoutMs, std::vector<Channel*>* pOutActiveChannles);
    void updateChannel(Channel* pChannel);
    void removeChannel(Channel* pChannel);

    bool hasChannel(Channel* pChannel) const;
    void assertInLoopThread() const
    {
        return m_pLoop->assertInLoopThread();
    }
private:
    static const int kInitEventListsize = 16;
    static const char* operationToString(int op);
    void fillActiveChannels(int numEvents, std::vector<Channel*>* pOutActiveChannles) const;
    void update(int operation, Channel* pChannel);
    
private:
    EventLoop* m_pLoop;
    std::map<int, Channel*> m_mapChannels;

    int m_epollFd;
    std::vector<struct epoll_event> m_vecEvents;
};

#endif