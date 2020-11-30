CLANG =   clang   -Wall -Werror -Wthread-safety -g -std=c11 -I../
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../

all:
	@#$(CLANG++) 01test_Observer.cpp -o 01test_Observer.out
	@#$(CLANG++) 02test_Observer_safe.cpp -o 02test_Observer_safe.out
	@#$(CLANG++) ../CurrentThread.cpp 03test_factory1.cpp -o 03test_factory1.out
	@#$(CLANG++) ../CurrentThread.cpp 04test_factory2.cpp -o 04test_factory2.out
	@#$(CLANG++) ../CurrentThread.cpp 05test_factory3.cpp -o 05test_factory3.out
	@#$(CLANG++) ../CurrentThread.cpp 06test_factory4.cpp -o 06test_factory4.out
	@#$(CLANG++) ../CurrentThread.cpp 07test_facotry5.cpp -o 07test_facotry5.out
	@#$(CLANG++) 08test_non_recursive_mutex.cpp -o 08test_non_recursive_mutex.out -lpthread
	$(CLANG++) 09test_copy_on_write.cpp -o 09test_copy_on_write.out -lpthread


	@#$(CLANG) ../CurrentThread.cpp ../Thread.cpp ../CountDownLatch.cpp 01test_thread_factory1.cpp -o 01test_thread_factory1.out -lpthread
	@#$(CLANG) ../CurrentThread.cpp ../Timestamp.cpp  02test_thread_deadlock.cpp -o 02test_thread_deadlock.out -lpthread

clean:
	rm -fr *.out