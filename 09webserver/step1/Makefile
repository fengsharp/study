CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	@#$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp \
	#../Timestamp.cpp ../TimeZone.cpp ../Date.cpp \
	#../LogStream.cpp ../ProcessInfo.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp \
	#EventLoop.cpp \
	#test_event_loop_suc.cpp -o test_event_loop_suc.out \
	#-lpthread

	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp \
	../Timestamp.cpp ../TimeZone.cpp ../Date.cpp \
	../LogStream.cpp ../ProcessInfo.cpp ../LogFile.cpp ../FileUtil.cpp ../Logging.cpp \
	EventLoop.cpp \
	test_event_loop_fail.cpp -o test_event_loop_fail.out \
	-lpthread

clean:
	rm -fr *.out