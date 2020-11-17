CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp \
	../Timestamp.cpp ../TimeZone.cpp ../Date.cpp \
	../LogStream.cpp ../ProcessInfo.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp \
	Channel.cpp Poller.cpp EventLoop.cpp Timer.cpp TimerQueue.cpp \
	test_timer.cpp -o test_timer.out \
	-lpthread

clean:
	rm -fr *.out