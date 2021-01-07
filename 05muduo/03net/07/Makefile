CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../Timestamp.cpp ../LogStream.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp EventLoop.cpp Poller.cpp Channel.cpp TimerQueue.cpp EventWakeQueue.cpp SocketsOps.cpp Acceptor.cpp EventLoopThread.cpp 07test.cpp -o 07test.out -lpthread

clean:
	rm -fr *.out