#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buf[BUFSIZ] = "1001 feng 34\n";
    char buf2[BUFSIZ] = "1001 feng 34\n";

    int no = 0;
    char name[BUFSIZ];
    int age = 0;
    // 最好判断一下返回值
    sscanf(buf, "%d %s %d", &no, name, &age);
    printf("no:%d name:%s age:%d\n", no, name, age);

    puts("--- buf2 ---");
    sscanf(buf2, "%d %s %d", &no, name, &age);
    printf("no:%d name:%s age:%d\n", no, name, age);

    return 0;
}