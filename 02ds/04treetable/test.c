#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "treetable.h"

#define UNUSED_ATTRIBUTE __attribute__((unused))

int* int_create(int val) {
    int* ret = malloc(sizeof(int));
    *ret = val;
    return ret;
}

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int  int_cmp(const void *key1, const void *key2)
{
    return *(int*)key1 - *(int*)key2;
}

typedef int (*CalFunc)(int a, int b);

int main()
{
    TreeTable* table;
    treetable_new(int_cmp, &table);

    treetable_add(table, int_create(100), add);
    treetable_add(table, int_create(200), sub);

    int calIndex = 100;
    puts("callback 100");
    CalFunc callback;
    if (treetable_get(table, &calIndex, (void**)(&callback)) == CC_OK) {
        printf("callback 100: %d\n", callback(10, 5));
    }

    puts("callback 200");
    calIndex = 200;
    if (treetable_get(table, &calIndex, (void**)(&callback)) == CC_OK) {
        printf("callback 200: %d\n", callback(10, 5));
    }

    TreeTableEntry entry;
    TREETABLE_FOREACH(entry, table, {
        int* pKey = (int*)entry.key;
        *pKey += 1000;
        printf("## query key %d\n", *pKey);
    })

    TREETABLE_FOREACH(entry, table, {
        int* pKey = (int*)entry.key;
        printf("-- free key %d\n", *pKey);
        free(entry.key);
    })

    treetable_destroy(table);

    return 0;
}