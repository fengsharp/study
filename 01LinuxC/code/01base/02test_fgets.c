#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE* pFile = fopen("02test_fgets.c", "r");
    if (pFile == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char lineBuf[BUFSIZ];
    while (fgets(lineBuf, BUFSIZ, pFile) != NULL) {
        // 1.lineBuf一定有一个尾零
        // 2.如果有换行符，则会保留
        fprintf(stdout, "%s", lineBuf);
    }

    fclose(pFile);

    return 0;
}