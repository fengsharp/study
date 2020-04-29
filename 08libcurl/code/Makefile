all:
	@#gcc -Wall -g -std=c11 01easy.c -o 01easy.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 02writefunction.cpp -o 02writefunction.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 03multi.cpp -o 03multi.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 04cookie.cpp -o 04cookie.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 05multi_cookie.cpp -o 05multi_cookie.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 06cookie_list.cpp -o 06cookie_list.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 07multi_cookie_list.cpp -o 07multi_cookie_list.out -I/usr/local/curl -lcurl
	@#g++ -Wall -g -std=c++17 09myselfcookie.cpp -o 09myselfcookie.out -I/usr/local/curl -lcurl
	g++ -Wall -g -std=c++11 CookieCache.cpp 10test_cookiecach.cpp -o 10test_cookiecach.out -I/usr/local/curl -lcurl
	@#gcc -Wall -g -std=c++11 11test_timestamp.cpp -o 11test_timestamp.out
	
clean:
	rm *.out