CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 -I../
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../

all:
	$(CLANG++) ../Timestamp.cpp ../CurrentThread.cpp ../Thread.cpp ../Logging.cpp ../LogStream.cpp EventLoop.cpp Channel.cpp Poller.cpp TimerQueue.cpp WeakupQueue.cpp test.cpp -o  test.out -lpthread
clean:
	rm -fr *.o *.out