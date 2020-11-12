CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Timestamp.cpp ../Thread.cpp ../LogFile.cpp ../Logging.cpp ../LogStream.cpp ../ProcessInfo.cpp ../FileUtil.cpp ../TimeZone.cpp ../Date.cpp ../CountDownLatch.cpp EventLoop.cpp Poller.cpp Channel.cpp test02.cpp -o test02.out -lpthread


clean:
	rm -fr *.out