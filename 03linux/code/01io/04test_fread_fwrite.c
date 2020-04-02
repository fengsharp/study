#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char* strFileSrc = "./04test_fread_fwrite.c";
    const char* strFileDest = "./04copy.del";
    FILE* pSrc = fopen(strFileSrc, "r"); // check null
    FILE* pDest = fopen(strFileDest, "w"); // check null

    char buf[BUFSIZ];
    size_t ret = -1;
    while ((ret = fread(buf, 1, BUFSIZ, pSrc)) > 0) {
        fwrite(buf, 1, ret, pDest);
    }

    fclose(pDest);
    fclose(pSrc);
    return 0;
}