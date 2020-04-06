all:
	@#g++ -Wall -g -std=c++17 -I../ test_01_timestamp.cpp ../Timestamp.cpp -o test_01_timestamp.out
	@#g++ -Wall -g -std=c++17 -I../ test_02_atomic.cpp -o test_02_atomic.out
	g++ -Wall -g -std=c++17 -I../ test_03_Exception.cpp ../Exception.cpp ../CurrentThread.cpp -o test_03_Exception.out
	
clean:
	rm *.out