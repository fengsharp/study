CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../Timestamp.cpp ../LogStream.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp EventLoop.cpp Channel.cpp Poller.cpp TimerQueue.cpp PendingQueue.cpp EventLoopThread.cpp InetAddress.cpp Acceptor.cpp Socket.cpp SocketsOpts.cpp 05test.cpp -o 05test.out -lpthread


clean:
	rm -fr *.out