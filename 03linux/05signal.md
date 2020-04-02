# 1.发信号
异步方案：  
1. 轮询查询
2. 回调通知  

typedef void (*sighandler_t)(int);  
sighandler_t signal(int signum, sighandler_t handler);  
信号会打断阻塞的系统调用  

# 2.信号不可靠
# 3.可重入函数
所有系统调用都是可重入，一部分库函数是可重入，如memcpy

# 4.信号的响应过程
# 5.常用 
kill raise alarm pause abort system 
# 6.信号集
# 7.信号屏蔽字
# 8.sigsuspend sigaction setitimer
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);  

# 9.实时信号