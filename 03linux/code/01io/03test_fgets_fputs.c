#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char* strFileSrc = "./03test_fgets_fputs.c";
    const char* strFileDest = "./03copy.del";
    FILE* pSrc = fopen(strFileSrc, "r"); // check null
    FILE* pDest = fopen(strFileDest, "w"); // check null

    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, pSrc) != NULL) {
        fputs(buf, pDest);
    }

    fclose(pDest);
    fclose(pSrc);

    return 0;
}