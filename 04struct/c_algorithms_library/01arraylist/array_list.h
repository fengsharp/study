#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct _ArrayList;
typedef struct _ArrayList ArrayList;

typedef void (*ArryListDataFreeFunc)(void* ptr);
typedef int (*ArryListDataCmpFunc)(void* ctx, void* element);

ArrayList* array_list_create(ArryListDataFreeFunc dataFreeFunc);
void array_list_destroy(ArrayList* thiz);

bool array_list_insert(ArrayList* thiz, unsigned int index, void* data);
bool array_list_append(ArrayList* thiz, void* data);
bool array_list_prepend(ArrayList* thiz, void* data);

void array_list_delete_range(ArrayList* thiz, unsigned int index, unsigned int len);
void array_list_delete(ArrayList* thiz, unsigned int index);

int array_list_index_of(ArrayList* thiz, void* data, ArryListDataCmpFunc cmpFunc);

void array_list_clear(ArrayList* thiz);

unsigned int array_list_size(ArrayList* thiz);
unsigned int array_list_capacity(ArrayList* thiz);

void* array_list_value(ArrayList* thiz, unsigned int index);

#ifdef __cplusplus
}
#endif

#endif