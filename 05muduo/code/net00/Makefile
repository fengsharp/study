CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 -I../
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../

all:
	$(CLANG++) ../Timestamp.cpp ../CurrentThread.cpp ../Thread.cpp ../Logging.cpp ../LogStream.cpp EventLoop.cpp test01.cpp -o  test01.out -lpthread
clean:
	rm -fr *.o *.out