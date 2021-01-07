CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../Timestamp.cpp ../LogStream.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp EventLoop.cpp Poller.cpp Channel.cpp TimerQueue.cpp EventWakeQueue.cpp SocketsOps.cpp Acceptor.cpp 06test.cpp -o 06test.out -lpthread

clean:
	rm -fr *.out