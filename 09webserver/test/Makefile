CLANG = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 -I../
all:
	@#$(CLANG) test01_Timestamp_unittest.cpp ../Timestamp.cpp -o test01_Timestamp_unittest.out
	@#$(CLANG) test02_Atomic_unittest.cpp -o test02_Atomic_unittest.out
	@#$(CLANG) ../CurrentThread.cpp ../Exception.cpp test03_Exception.cpp -o test03_Exception.out
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp ../Exception.cpp test04_Thread.cpp -o test04_Thread.out -lpthread
	@#$(CLANG) ../Timestamp.cpp ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp ../Exception.cpp test05_Thread_bench.cpp -o test05_Thread_bench.out -lpthread
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test06_Singleton.cpp -o test06_Singleton.out  -lpthread
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test07_ThreadLocal.cpp -o test07_ThreadLocal.out  -lpthread
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test08_SingletonThreadLocal.cpp -o test08_SingletonThreadLocal.out  -lpthread
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test09_ThreadLocalSingleton.cpp -o test09_ThreadLocalSingleton.out  -lpthread
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test10_BlockQueue.cpp -o test10_BlockQueue.out  -lpthread
	@#$(CLANG) ../Timestamp.cpp ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test11_BlockQueue_bench.cpp -o test11_BlockQueue_bench.out  -lpthread
	@#$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp test12_BoundedBlockingQueue.cpp -o test12_BoundedBlockingQueue.out  -lpthread
	$(CLANG) ../Condition.cpp ../CountDownLatch.cpp ../Thread.cpp ../CurrentThread.cpp ../ThreadPool.cpp test13_ThreadPool.cpp -o test13_ThreadPool.out  -lpthread


clean:
	rm -fr *.out