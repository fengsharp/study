CLANG =   clang   -Wall -Werror -Wthread-safety -g -std=c11 -I../
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../

all:
	@#$(CLANG++) 01test_Observer.cpp -o 01test_Observer.out
	$(CLANG++) 02test_Observer_safe.cpp -o 02test_Observer_safe.out

	@#$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp 01test_thread_factory1.cpp -o 01test_thread_factory1.out -lpthread
	@#$(CLANG) ../CurrentThread.cpp ../Timestamp.cpp  02test_thread_deadlock.cpp -o 02test_thread_deadlock.out -lpthread

clean:
	rm -fr *.out