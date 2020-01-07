#ifndef DLIST_H_
#define DLIST_H_

#include "common.h"

struct _DList;
typedef struct _DList DList;

typedef void (*DListEleDestroyFunc)(void* element);
typedef bool (*DListCompareFunc)(const void* pctx, const void* element);
typedef void (*DListVisitFunc)(const void* pctx, void* element);

DList* dlist_create(DListEleDestroyFunc eleDestroyFunc);

bool dlist_push_back(DList* thiz, void* data);
const void* dlist_find(DList* thiz, void* pctx, DListCompareFunc compareFunc);

void dlist_foreach(DList* thiz, void* pctx, DListVisitFunc visitFunc);

size_t dlist_get_len(DList* thiz);

void dlist_destroy(DList* thiz);

#endif