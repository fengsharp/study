CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 

all:
	@#${CLANG++} 01test.cpp -o 01test.out
	${CLANG++} 02test.cpp -o 02test.out

clean:
	rm -fr *.out