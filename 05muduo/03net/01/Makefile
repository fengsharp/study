CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../Timestamp.cpp ../LogStream.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp EventLoop.cpp 01test.cpp -o 01test.out -lpthread


clean:
	rm -fr *.out