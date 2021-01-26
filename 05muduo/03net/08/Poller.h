#ifndef POLLER_H_
#define POLLER_H_

#include <vector>

#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;
class Channel;
struct epoll_event;

class Poller : private NonCopyable
{
public:
    explicit Poller(EventLoop * pLoop);
    ~Poller();

    Timestamp poll(int timeoutMs, std::vector<Channel *> * pOutActiveChannels);
    void updateChannel(Channel * pChannel);
private:
    void update(int op, Channel * pChannel);
private:
    EventLoop * m_pLoop;
    int m_epollfd;
    std::vector<struct epoll_event> m_vecPollfdList;
};

#endif