all:
	@#g++ -Wall -g -std=c++17 -I../ test_01_timestamp.cpp ../Timestamp.cpp -o test_01_timestamp.out
	@#g++ -Wall -g -std=c++17 -I../ test_02_atomic.cpp -o test_02_atomic.out
	@#g++ -Wall -g -std=c++17 -I../ test_03_Exception.cpp ../Exception.cpp ../CurrentThread.cpp -o test_03_Exception.out
	@#g++ -Wall -g -std=c++17 -I../ test_04_thread.cpp ../*.cpp -o test_04_thread.out -lpthread
	@#g++ -Wall -g -std=c++17 -I../ test_05_thread_bench.cpp ../*.cpp -o test_05_thread_bench.out -lpthread
	@#g++ -Wall -g -std=c++17 -I../ test_06_mutex.cpp ../*.cpp -o test_06_mutex.out -lpthread
	@#g++ -Wall -g -std=c++17 -I../ test_07_BlockQueue.cpp ../*.cpp -o test_07_BlockQueue.out -lpthread
	@#g++ -Wall -g -std=c++17 -I../ test_08_BoundedBlockingQueue.cpp ../*.cpp -o test_08_BoundedBlockingQueue.out -lpthread
	g++ -Wall -g -std=c++17 -I../ test_09_ThreadPool.cpp ../*.cpp -o test_09_ThreadPool.out -lpthread
	
clean:
	rm *.out