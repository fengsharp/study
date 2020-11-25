CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	@#$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp 01test_thread_factory1.cpp -o 01test_thread_factory1.out -lpthread
	@#$(CLANG) ../CurrentThread.cpp ../Timestamp.cpp  02test_thread_deadlock.cpp -o 02test_thread_deadlock.out -lpthread
	$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp ../Timestamp.cpp  03test_thread_multi_dead.cpp -o 03test_thread_multi_dead.out -lpthread

clean:
	rm -fr *.out