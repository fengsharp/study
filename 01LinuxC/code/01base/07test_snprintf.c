#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buffer[6];
    //char* s = "runoo bcom";
    char s[] = {'r', 'u', 'n', 'o', 'o', 'b', 'c', 'o', 'm'};
    char* s1 = "jalsdjflaksjdflakjdsflakjsfd";
 
    // 读取字符串并存储在 buffer 中
    int j = snprintf(buffer, 6, "%s ", s);
 
    // 输出 buffer及字符数
    printf("string:\n%s\ncharacter count = %d\n", buffer, j);

    return 0;
}