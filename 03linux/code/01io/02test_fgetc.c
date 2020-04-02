#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char* strFileSrc = "./02test_fgetc.c";
    FILE* pSrc = fopen(strFileSrc, "r"); // check null

    int cnt = 0;
    int ret = EOF;
    while ((ret = fgetc(pSrc)) != EOF) {
        ++cnt;
    }

    fclose(pSrc);

    fprintf(stdout, "[%s] size: %d\n", strFileSrc, cnt);

    return 0;
}