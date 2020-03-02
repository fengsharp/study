#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buf[BUFSIZ] = "a100*b200*c300**d400**";
    const char* delim = "*";

    char* findStr = strtok(buf, delim);
    while (findStr != NULL) {
        if (strcmp(findStr, delim) != 0) {
            puts(findStr);
        }
        findStr = strtok(NULL, delim);
    }

    return 0;
}