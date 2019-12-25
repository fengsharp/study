# utility
1. 获取随机值  
```c
#include <time.h>
time_t time(time_t* t);

#include <stdlib.h>
void srand(unsigned int seed);
int rand(void);

srand(time(NULL));
int r = rand();
```


# string
1. 字符串获取
    ```c
    char* fgets(char* s, int size, FILE* stream);
    ```
    stdin stdout stderr  
    需要特别强调  
    1. fgets 读取到结尾 or \n or size-1 会结束，并追加一个\0
    2. 如果是从键盘输入，遇到\n结束，则fgets会将\n带入到s中
2. 字符串输出
    ```c
    int puts(const char* s); // 自动追加了\n
    int fputs(const char* str, FILE* stream); // 特别注意，字符串中的\0不会写入文件
    ```
3. 长度
    ```c
    size_t strlen(const char* s);
    // size_t - unsigned int
    ```
4. 赋值或者复制
    ```c
    char* strncpy(char* dest, const char* src, size_t n);
    ```
    需要特别注意的地方，fgets会在s末尾自动追加一个0  
    而strncpy不会，单纯的在src中找到前n个字符拷贝到dest中  
    所以在使用时，要确定dest至少有n+1个空间，src至少有n个空间  
    且在结束后，dest[n-1]='0'  
    如果全部拷贝，可以使用strlen(src)，不要忘记考虑是否需要dest尾零
5. 追加
    ```c
    char* strncat(char* dest, const char* src, size_t n);
    ```
    需要注意的是，n个字符过去后，会自动追加0  
    所以dest要至少n+1的空间大小

6. 对比
    ```c
    int strncmp(const char* s1, const char* s2, size_t n);
    ```
7. 格式化
    ```c
    int sprintf(char* str, const char* format, ...);
    int sscanf(const char* str, const char* format, ...);
    ```
8. 查找
    ```c
    char* strchr(const char* s, int c);
    suc: 返回c第一次出现的位置  
    fail: NULL  

    char* strstr(const char* haystack, const char* needle);
    suc: 第一次出现needle的位置
    fail: NULL
    ```
9. 分割字符串
    ```c
    char* strtok(char* str, const char* delim);
    suc: 分割后字符串首地址
    fail: NULL
    需要注意：当连续出现多个时，只替换第一个位\0
    另外第一次调用传入str，后续的分割仅传入NULL即可

    char* s = strtok(src, "*");
    while (s != NULL) {
        if (*s == '*') {
            continue;
        }

        puts(s);
        s = strtok(NULL, "*");
    }
    ```

10. 字符转数值
    ```c
    int atoi(const char* nptr);
    atof atol
    ```
    注意：  
    扫描npt，跳过空格，遇到数字或正负号开始，尾零或者非数字结束

# memory
1. memset
    ```c
    void* memset(void* s, int c, size_t n);
    ```

2. memcpy
    ```c
    void* memcpy(void* dest, void* src, size_t n);
    ```
    注意：不能dest和src重叠(memmove支持，但是不重叠效率没有memcpy高)

3. memcmp
    ```c
    int memcmp(const void* s1, const void* s2, size_t n);
    ```

4. malloc and free
    void* malloc(size_t n);  
    void free(void* ptr);  

# IO
1. 文件打开关闭
    fopen fclose

2. 按照字符读写
    fgetc fputc

3. 按照行读写
    fgets fputs

4. 按照格式化读写
    fprintf fscanf

5. 按照块读写
    fread fwrite

6. 随机读写
    fseek

8. 缓冲刷新
    fflush