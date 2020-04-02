all:
	@#gcc -Wall -g -std=c11 01test_primer.c -o 01test_primer.out -lpthread
	@#gcc -Wall -g -std=c11 02test_write_num.c -o 02test_write_num.out -lpthread -D_BSD_SOURCE
	gcc -Wall -g -std=c11 03test_pool.c -o 03test_pool.out -lpthread -D_BSD_SOURCE
	
clean:
	rm *.out