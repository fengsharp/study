1. clang的编译线程安全注解  
https://www.cnblogs.com/cherishui/p/10962161.html  
https://www.cnblogs.com/jicanghai/p/9472001.html  

使用： 
    1. code
    guarded_by 一个线程在读写变量前，必须锁住资源  
    requires   调用方法前，必须锁住了对应变量
    acquire  
    release  
    2. clang -Wthread-safety   
    
2. step
- 听 看
- 实践
- 总结
- 写（想->实践）
- 讲

25记录：
1. 网络编程三件事情：
- 建立连接的过程 server-accept   client-connect
- 关闭连接  close shutdown read->0
- 读取数据  readable
- 写入数据  
2. 目标
EventLoop

26 27记录：
- Poller
- Channel

28记录
- Timer