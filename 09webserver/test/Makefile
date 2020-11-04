CLANG = clang++ -Wall -Werror -g -std=c++17 -I../
all:
	@#$(CLANG) test01_Timestamp_unittest.cpp ../Timestamp.cpp -o test01_Timestamp_unittest.out
	$(CLANG) test02_Atomic_unittest.cpp -o test02_Atomic_unittest.out

clean:
	rm -fr *.out