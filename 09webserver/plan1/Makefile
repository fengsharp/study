CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Timestamp.cpp ../Thread.cpp ../LogFile.cpp ../Logging.cpp ../LogStream.cpp ../ProcessInfo.cpp EventLoop.cpp ../FileUtil.cpp ../TimeZone.cpp ../Date.cpp ../CountDownLatch.cpp test01.cpp -o test01.out -lpthread


clean:
	rm -fr *.out