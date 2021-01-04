CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../Timestamp.cpp ../LogStream.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp EventLoop.cpp Channel.cpp Poller.cpp TimerQueue.cpp 04test.cpp -o 04test.out -lpthread


clean:
	rm -fr *.out