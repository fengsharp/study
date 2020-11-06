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
    
2. 