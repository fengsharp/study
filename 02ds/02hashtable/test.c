#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

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
    HashTable* table;
    HashTableConf conf;
    hashtable_conf_init(&conf);
    conf.key_compare = int_cmp;
    hashtable_new_conf(&conf, &table);

    hashtable_add(table, int_create(100), add);
    hashtable_add(table, int_create(200), sub);

    int calIndex = 100;
    puts("callback 100");
    CalFunc callback;
    if (hashtable_get(table, &calIndex, (void**)(&callback)) == CC_OK) {
        printf("callback 100: %d\n", callback(10, 5));
    }

    puts("callback 200");
    calIndex = 200;
    if (hashtable_get(table, &calIndex, (void**)(&callback)) == CC_OK) {
        printf("callback 200: %d\n", callback(10, 5));
    }

    void* itemKey = NULL;
    void* UNUSED_ATTRIBUTE itemValue = NULL;
    HASHTABLE_FOREACH(table, itemKey, itemValue, {
        int* pKey = (int*)itemKey;
        printf("-- free key %d\n", *pKey);
        free(itemKey);
    })

    hashtable_destroy(table);

    return 0;
}