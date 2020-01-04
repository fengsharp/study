#ifndef DATA_MANAGER_H_
#define DATA_MANAGER_H_

#include "dlist.h"

struct _DataManager;
typedef struct _DataManager DataManager;

DataManager* dm_create(const char* datafile);

void dm_get_all(DataManager* thiz, DList* dlist);

void dm_destroy(DataManager* thiz);

#endif