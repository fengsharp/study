#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

void print_int_free(void* target) {
    int* pValue = (int*)target;
    printf(" %d", *pValue);
    free(target);
}

int int_cmp(const void* e1, const void* e2) {
    int** pLhs = (int**)e1;
    int** pRhs = (int**)e2;
    // printf("--- %d - %d = %d ---\n", **pLhs, **pRhs, (**pLhs) - (**pRhs));
    return (**pLhs) - (**pRhs);
}

void int_add_one(void* e) {
    int* pValue = (int*)e;
    *pValue += 1;
}

int main()
{
    Array* pAr;
    if (array_new(&pAr) != CC_OK) {
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<10; ++i) {
        int* tmp = malloc(sizeof(int));
        *tmp = i;
        array_add(pAr, tmp);
    }

    printf("for index output:");
    for (int i=0; i<array_size(pAr); ++i) {
        int* pTmp;
        array_get_at(pAr, i, (void**)(&pTmp));
        printf(" %d", *pTmp);
    }
    printf("\n");

    {
        int* pNewVal = malloc(sizeof(int));
        *pNewVal = 5;

        int* pOldVal = NULL;
        array_replace_at(pAr, pNewVal, 9, (void*)(&pOldVal));
        free(pOldVal);
    }

    array_sort(pAr, int_cmp);
    array_map(pAr, int_add_one);

    printf("array foreach:");
    void* cursor;
    ARRAY_FOREACH(cursor, pAr, {
        int* pTmp = (int*)cursor;
        printf(" %d", *pTmp);
    })
    printf("\n");

    printf("destory:");
    array_destroy_cb(pAr, print_int_free);
    printf("\n");

    return 0;
}