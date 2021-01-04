#ifndef POLLER_H_
#define POLLER_H_

#include <vector>

#include "NonCopyable.h"
#include "Timestamp.h"

class Channel;
class EventLoop;
struct epoll_event;

class Poller : private NonCopyable
{
public:
    explicit Poller(EventLoop * pLoop);
    ~Poller();

    Timestamp poll(int waitMs, std::vector<Channel*> * pOutActiveChannels);

    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);
private:
    void updateChannel(int op, Channel * pChanne);

private:
    EventLoop * m_pLoop;
    int m_epollid;
    
    std::vector<struct epoll_event> m_vecTmpContainer;
};

#endif