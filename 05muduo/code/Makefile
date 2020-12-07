CLANG = clang -Wall -Werror -Wthread-safety -g -std=c11 
CLANG++ = clang++ -Wall -Werror -Wthread-safety -g -std=c++17 

all:
	@#$(CLANG++) 01test_noncopyable.cpp -o 01test_noncopyable.out
	@#$(CLANG++) Timestamp.cpp 02test_timestamp.cpp -o 02test_timestamp.out
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp 03test_thread.cpp -o 03test_thread.out -lpthread
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp 04test_mutext.cpp -o 04test_mutext.out -lpthread
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp 05test_atomic.cpp -o 05test_atomic.out -lpthread
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp 06test_latch1.cpp -o 06test_latch1.out -lpthread
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp 07test_latch2.cpp -o 07test_latch2.out -lpthread
	@#$(CLANG++) 08test_move.cpp -o 08test_move.out
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp  09test_blockqueue.cpp -o  09test_blockqueue.out -lpthread
	
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp Logging.cpp LogStream.cpp EventLoop.cpp 10test_reactor01.cpp -o  10test_reactor01.out -lpthread
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp Logging.cpp LogStream.cpp Channel.cpp Poller.cpp EventLoop.cpp 11test_reactor02.cpp -o  11test_reactor02.out -lpthread
	@#$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp Logging.cpp LogStream.cpp Channel.cpp Poller.cpp EventLoop.cpp Timer.cpp TimerQueue.cpp 12test_reactor03.cpp -o  12test_reactor03.out -lpthread
	$(CLANG++) Timestamp.cpp CurrentThread.cpp Thread.cpp Logging.cpp LogStream.cpp Channel.cpp Poller.cpp EventLoop.cpp Timer.cpp TimerQueue.cpp 13test_reactor04.cpp -o  13test_reactor04.out -lpthread
clean:
	rm -fr *.o *.out