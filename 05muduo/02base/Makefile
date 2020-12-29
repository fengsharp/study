CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 

all:
	@#$(CLANG++) 01test_Copy_Noncopy.cpp -o 01test_Copy_Noncopy.out
	@#$(CLANG++) 02test_types_cast.cpp -o 02test_types_cast.out
	@#$(CLANG++) Timestamp.cpp 03test_timestamp.cpp -o 03test_timestamp.out
	@#$(CLANG++) 04test_atomic.cpp -o 04test_atomic.out
	@#$(CLANG++) Thread.cpp CurrentThread.cpp Timestamp.cpp 05test_blocking_queue.cpp -o 05test_blocking_queue.out -lpthread
	$(CLANG++) Thread.cpp CurrentThread.cpp Timestamp.cpp 06test_singleton.cpp -o 06test_singleton.out -lpthread

clean:
	rm -f ./*.out ./*.o