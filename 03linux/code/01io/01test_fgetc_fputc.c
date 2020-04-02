#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char* strFileSrc = "./01test_fgetc_fputc.c";
    const char* strFileDest = "./01copy.del";
    FILE* pSrc = fopen(strFileSrc, "r"); // check null
    FILE* pDest = fopen(strFileDest, "w"); // check null

    int ret = EOF;
    while ((ret = fgetc(pSrc)) != EOF) {
        fputc(ret, pDest);
    }

    fclose(pDest);
    fclose(pSrc);

    return 0;
}