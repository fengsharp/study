#include <stdio.h>

#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <functional>

#include "EventLoop.h"
#include "CurrentThread.h"
#include "Channel.h"
#include "Types.h"

EventLoop * g_pLoop = NULL;
int timerfd;

void timeout(Timestamp receiveTime)
{
	printf("Timeout!\n");
	uint64_t howmany;
	::read(timerfd, &howmany, sizeof howmany);
	g_pLoop->stop();
}

int main()
{
    EventLoop loop;
    g_pLoop = &loop;

    timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timerfd);
	channel.setReadCallback(std::bind(&timeout, std::placeholders::_1));
	channel.enableReading();

	struct itimerspec howlong;
	memZero(&howlong, sizeof howlong);
	howlong.it_value.tv_sec = 1;
	::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    puts("=== ok. ===");

    ::close(timerfd);
    
    return 0;
}