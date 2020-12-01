CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 

all:
	$(CLANG++) 01test_noncopyable.cpp -o 01test_noncopyable.out
	
clean:
	rm -fr *.o *.out