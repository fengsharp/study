#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 5
int main()
{
    char buf[BUF_LEN];
    char* dest = "hello,world";
    // 正确处理strncpy的n值问题，或者判断strlen(dest) 与 BUF_LEN的关系
    strncpy(buf, dest, BUF_LEN-1);
    buf[BUF_LEN-1] = '\0';
    printf("%s, len:%lu\n", buf, strlen(buf));

    return 0;
}