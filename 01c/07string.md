## strncpy
```c
    char str1[] = "abc";
    char buf[2] = {'\0'};

    strncpy(buf, str1, sizeof(buf)-1);
    puts(buf);
```
strncpy 的size一定用容器大小-1，且要保证加上了尾0

## sscanf
```c
const char* range = "0-12";
size_t start = 0;
size_t end = 0;
int ret = EOF;
if ((ret = sscanf(range, "%lu-%lu", &start, &end)) != 2)
{
    printf("convert error. start:%lu, end:%lu, ret:%d\n", start, end, ret);
} else {
    printf("start:%lu, end:%lu, ret:%d\n", start, end, ret);
}
```
留意对sscanf 返回值的判断

## snprintf
```c
int snprintf ( char * s, size_t n, const char * format, ... );
```
1. 最多会处理n-1个字符  
2. 自动追加一个换行符  
3. ret >=0 && ret < n 说明format后面的内容完全缓存到了s中  

