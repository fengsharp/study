CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 

all:
	@#$(CLANG++) 01test_Copy_Noncopy.cpp -o 01test_Copy_Noncopy.out
	@#$(CLANG++) 02test_types_cast.cpp -o 02test_types_cast.out
	@#$(CLANG++) Timestamp.cpp 03test_timestamp.cpp -o 03test_timestamp.out
	$(CLANG++) 04test_atomic.cpp -o 04test_atomic.out

clean:
	rm -f ./*.out ./*.o