#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

void print_int_free(void* target) {
    int* pValue = (int*)target;
    printf(" %d", *pValue);
    free(target);
}

int int_cmp(const void* e1, const void* e2) {
    int** pLhs = (int**)e1;
    int** pRhs = (int**)e2;
    // printf("--- %d - %d = %d ---\n", **pLhs, **pRhs, (**pLhs) - (**pRhs));
    return -(**pLhs) + (**pRhs);
}

int main()
{
    List* pList;
    if (list_new(&pList) != CC_OK) {
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<10; ++i) {
        int* tmp = malloc(sizeof(int));
        *tmp = i;
        list_add_last(pList, tmp);
    }

    printf("for index output:");
    for (int i=0; i<list_size(pList); ++i) {
        int* pTmp;
        list_get_at(pList, i, (void**)(&pTmp));
        printf(" %d", *pTmp);
    }
    printf("\n");

    list_sort(pList, int_cmp);

    printf("list foreach:");
    void* cursor;
    LIST_FOREACH(cursor, pList, {
        int* pTmp = (int*)cursor;
        printf(" %d", *pTmp);
    })
    printf("\n");

    printf("destory:");
    list_destroy_cb(pList, print_int_free);
    printf("\n");

    return 0;
}