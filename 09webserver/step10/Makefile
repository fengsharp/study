CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp \
	../Timestamp.cpp ../TimeZone.cpp ../Date.cpp \
	../LogStream.cpp ../ProcessInfo.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp \
	Channel.cpp Poller.cpp EventLoop.cpp Timer.cpp TimerQueue.cpp EventLoopThread.cpp \
	InetAddress.cpp SocketsOpts.cpp Socket.cpp Acceptor.cpp \
	TcpConnection.cpp TcpServer.cpp \
	test_server_close.cpp -o test_server_close.out \
	-lpthread

clean:
	rm -fr *.out