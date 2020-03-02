#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char* str1 = "hello";
    const char* str2 = "world";
    const char* str3 = "hello, ";
    const char* str4 = "hello";

    printf("str1:%p, str4:%p\n", str1, str4);
    printf("[%s] - [%s], strncmp:%d\n", str1, str2, strncmp(str1, str2, strlen(str1)));
    printf("[%s] - [%s], strncmp:%d\n", str1, str3, strncmp(str1, str3, strlen(str1)));
    printf("[%s] - [%s], strncmp:%d\n", str1, str4, strncmp(str1, str4, strlen(str1)));
    puts("");
    printf("[%s] - [%s], strcmp:%d\n", str1, str2, strcmp(str1, str2));
    printf("[%s] - [%s], strcmp:%d\n", str1, str3, strcmp(str1, str3));
    printf("[%s] - [%s], strcmp:%d\n", str1, str4, strcmp(str1, str4));

    return 0;
}
/*
    str1:0x400870, str4:0x400870
    [hello] - [world], strncmp:-15
    [hello] - [hello, ], strncmp:0
    [hello] - [hello], strncmp:0

    [hello] - [world], strcmp:-15
    [hello] - [hello, ], strcmp:-44
    [hello] - [hello], strcmp:0
*/