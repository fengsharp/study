# 1.目录和文件
1. 获取文件属性  
int stat(const char *pathname, struct stat *buf);
2. 文件访问权限
3. umask
4. 文件权限的更改管理  
chmod 
5. 粘住位  
t位  
drwxrwxrwt.  13 root root 4096 Apr  2 16:17 tmp
6. 文件系统  
inode位图 | 块位图 | inode | block  
```c
struct inode {
    stat
    亚数据
    Block* arrBlock[15]; // 前12个直接指向block后面的记录到block里面，记录指向其他的block的地址
}
```

7. link soft link  
ln -s srcFile destPath

8. utime  
9. 目录的创建和销毁  
mkdir rmdir
10. 更改当前工作路径  
chdir getcwd
11. 读取目录内容  
opendir closedir readdir 
rewinddir seekdir telldir

# 2.系统数据文件和信息
1. /etc/passwd  
    getpwuid getpwnam -> passwd*
2. /etc/group  
    getgrgid getgrgrnam -> group*
3. /etc/shadow  
    getspnam -> spwd*  
    crypt  
    getpass  
4. time  
time() -> time_t  
time_t -> localtime() -> struct tm
struct tm -> mktime -> time_t
struct tm -> strftime() -> format string
string -> strptime() -> struct tm



# 3.进程环境
1. int atexit(void (*function)(void));  










