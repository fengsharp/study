# 记录C的语言疑惑
## 1.gcc
1. gcc 查看预处理
    gcc -E a.c -o a.i
2. gcc 汇编
    gcc -S a.c -o a.s
3. 查看依赖项
    ldd a.out

## 2.基本数据类型
1. 各种进制的表示方式
    十进制 10  
    十六进制 0x0a  
    八进制 012  
    二进制  不能直接书写二进制   
2. 打印进制
    %d %o %x
3. 打印short 
    %hd
4. size_t
    size_t - long unsigned int
    
## 3.补码
正数 原码表示，原码=反码=补码  
负数 补码 = 原码的反码+1  
 15 0000 1111  
-15 1111 0001 
    
## 4.常见开发C相关
### 1.获取随机值
    rand -> [0, RAD_MAX]  
    srand(time(NULL))  



## 5.字符串操作  
### 1.fgets
```c
char *fgets(char *s, int size, FILE *stream);
1. 读取字节范围[1, size-1]
2. 在字节范围内，如果碰到了\n 或者流结束标记EOF，完毕。如果是\n，会在读入\n后结束，并自动追加\0
3. 如果读取到了size-1，还没有正常结束，则停止。尾部自动加\0
```
### 2.strncpy
```c
带有str标记的string处理方法，涉及到N时，都不会自动处理尾零
char* strncpy(char* dest, const char* src, size_t n);

char buf[5];
strncpy(buf, "hello, world!", 4);
buf[4] = '\0';
```
### 3.strncat
```c
char *strncat(char *dest, const char *src, size_t n);
可能实现
char* strncat(char *dest, const char *src, size_t n)
{
    size_t dest_len = strlen(dest);
    for (size_t i = 0 ; i < n && src[i] != '\0' ; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';

    return dest;
}
显然，如果要追加，必须保证dest从尾零开始算，后面还需要有n+1的空间，如果n>strln(src) n=strln(src)
```

### 4. strcmp
```c
int strcmp(const char *s1, const char *s2);
int strncmp(const char* s1, const char* s2, size_t n);

strcmp 如果s1 s2长度不同，值也不为0
```

### 5. 其他类型构成字符串
```c
int snprintf(char *str, size_t size, const char *format, ...);

1. 最多会处理size-1个字符  
2. 自动追加一个换行符  
3. ret >=0 && ret < size 说明format后面的内容完全缓存到了s中  
```

### 6. 字符串类型，拆分出特定值
```c
int sscanf(const char *str, const char *format, ...);
如果成功，返回值会返回成功匹配的个数，使用时，严格意义来讲，返回值应该和要求的格式化参数数目一致
```

### 7. 子串在主串中首次出现的位置
```c
char* strchr(const char* s, int c);
    suc: 返回c第一次出现的位置  
    fail: NULL  

char *strstr(const char *haystack, const char *needle);
char *strcasestr(const char *haystack, const char *needle);
```

### 8. strtok
```c
char* strtok(char* str, const char* delim);
    suc: 分割后字符串首地址
    fail: NULL
    需要注意：
    1.str 不能是const char*。比如在数据区的字符串常量
    2.当连续出现多个delim时，某些平台只替换第一个为\0
    3.另外第一次调用传入str，后续的分割仅传入NULL即可

```

### 9.atoi
```c
int atoi(const char *nptr);
long atol(const char *nptr);
long long atoll(const char *nptr);
```

### 10.memory
```c
void *memset(void *s, int c, size_t n);
// 注意：不能dest和src重叠(memmove支持，但是不重叠效率没有memcpy高)
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
```
