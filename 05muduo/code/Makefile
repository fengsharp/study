CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 

all:
	@#$(CLANG++) 01test_noncopyable.cpp -o 01test_noncopyable.out
	$(CLANG++) Timestamp.cpp 02test_timestamp.cpp -o 02test_timestamp.out
	
clean:
	rm -fr *.o *.out