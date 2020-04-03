#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void err_exit(const char* err) {
    perror(err);
    exit(EXIT_FAILURE);
}

typedef struct _Range {
    int startIndex;
    int endIndex;
} Range;

Range* range_create(int sIndex, int eIndex) {
    Range* ret = malloc(sizeof(Range));
    if (ret == NULL) {
        err_exit("malloc range");
    }
    ret->startIndex = sIndex;
    ret->endIndex = eIndex;
    return ret;
}

void range_destroy(Range* thiz) {
    free(thiz);
}

void parse_range(FILE* pFile, Range* pArr[]) {
    int index = 0;
    do
    {
        int sIndex = -1;
        int ret = fscanf(pFile, "%d", &sIndex);
        if (ret == EOF || ret != 1 || sIndex < 0) {
            return;
        }

        int eIndex = -1;
        ret = fscanf(pFile, "%d", &eIndex);
        if (ret == EOF || ret != 1 || eIndex < 0) {
            return;
        }

        if (sIndex > eIndex) {
            exit(EXIT_FAILURE);
        }

        Range* pRange = range_create(sIndex, eIndex);
        if (pRange == NULL) {
            return;
        }
        pArr[index] = pRange;
        ++index;
    } while (1);
}

void destry_range(Range* pArr[]) {
    int index = 0;
    while (pArr[index] != NULL) {
        // printf("%d, %d\n", pArr[index]->startIndex, pArr[index]->endIndex);
        range_destroy(pArr[index]);
        ++index;
    }
}

int main()
{
    const char* fileName = "./test_data/01range.txt";
    FILE* pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        exit(EXIT_FAILURE);
    }

    Range* arr[BUFSIZ] = {NULL};
    parse_range(pFile, arr);

    char bufLine[BUFSIZ];
    while (fgets(bufLine, BUFSIZ, pFile) != NULL) {
        size_t size = strlen(bufLine);
        if (bufLine[size-1] == '\n') {
            bufLine[size - 1] = '\0'; // delete \n
            --size;
        }

        if (size == 0) {
            continue;
        }

        printf("--- %s\n", bufLine);
        
        int index = 0;
        while (1) {
            const Range* curItem = arr[index];
            ++index;
            if (curItem == NULL || curItem->startIndex >= size) {
                break;
            }

            char buf[BUFSIZ] = {'\0'};
            strncpy(buf, bufLine + curItem->startIndex, (curItem->endIndex - curItem->startIndex + 1));
            printf("%s", buf);
        }
        puts("");
    }

    destry_range(arr);
    fclose(pFile);

    puts("=== all over. ===");
    
    return 0;
}