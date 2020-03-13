#include <stdlib.h>
#include "dlist.h"

typedef struct _DListNode
{
	struct _DListNode* prev;
	struct _DListNode* next;

	void* data;
}DListNode;

struct _DList
{
	DListNode* first;
};

static DListNode* dlist_node_create(void* data)
{
	DListNode* node = malloc(sizeof(DListNode));

	if(node != NULL)
	{
		node->prev = NULL;
		node->next = NULL;
		node->data = data;
	}

	return node;
}

static void dlist_node_destroy(DListNode* node)
{
	if(node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		free(node);
	}

	return;
}

DList* dlist_create(void)
{
	DList* thiz = malloc(sizeof(DList));

	if(thiz != NULL)
	{
		thiz->first = NULL;
	}

	return thiz;
}

static DListNode* dlist_get_node(DList* thiz, size_t index, int fail_return_last)
{
	DListNode* iter = thiz->first;

	while(iter != NULL && iter->next != NULL && index > 0)
	{
		iter = iter->next;
		index--;
	}

	if(!fail_return_last)
	{
		iter = index > 0 ? NULL : iter;
	}

	return iter;
}

DListRet dlist_insert(DList* thiz, size_t index, void* data)
{
	DListNode* node = NULL;
	DListNode* cursor = NULL;

	if((node = dlist_node_create(data)) == NULL)
	{
		return DLIST_RET_OOM; 
	}

	if(thiz->first == NULL)
	{
		thiz->first = node;

		return DLIST_RET_OK;
	}

	cursor = dlist_get_node(thiz, index, 1);
	
	if(index < dlist_length(thiz))
	{
		if(thiz->first == cursor)
		{
			thiz->first = node;
		}
		else
		{
			cursor->prev->next = node;
			node->prev = cursor->prev;
		}
		node->next = cursor;
		cursor->prev = node;
	}
	else
	{
		cursor->next = node;
		node->prev = cursor;
	}

	return DLIST_RET_OK;
}

DListRet dlist_prepend(DList* thiz, void* data)
{
	return dlist_insert(thiz, 0, data);
}

DListRet dlist_append(DList* thiz, void* data)
{
	return dlist_insert(thiz, -1, data);
}

DListRet dlist_delete(DList* thiz, size_t index)
{
	DListNode* cursor = dlist_get_node(thiz, index, 0);

	if(cursor != NULL)
	{
		if(cursor == thiz->first)
		{
			thiz->first = cursor->next;
		}

		if(cursor->next != NULL)
		{
			cursor->next->prev = cursor->prev;
		}

		if(cursor->prev != NULL)
		{
			cursor->prev->next = cursor->next;
		}

		dlist_node_destroy(cursor);
	}

	return DLIST_RET_OK;
}

DListRet dlist_get_by_index(DList* thiz, size_t index, void** data)
{
	DListNode* cursor = dlist_get_node(thiz, index, 0);

	if(cursor != NULL)
	{
		*data = cursor->data;
	}

	return cursor != NULL ? DLIST_RET_OK : DLIST_RET_FAIL;
}

DListRet dlist_set_by_index(DList* thiz, size_t index, void* data)
{
	DListNode* cursor = dlist_get_node(thiz, index, 0);

	if(cursor != NULL)
	{
		cursor->data = data;
	}

	return cursor != NULL ? DLIST_RET_OK : DLIST_RET_FAIL;
}

size_t   dlist_length(DList* thiz)
{
	size_t length = 0;
	DListNode* iter = thiz->first;

	while(iter != NULL)
	{
		length++;
		iter = iter->next;
	}

	return length;
}

DListRet dlist_print(DList* thiz, DListDataPrintFunc print)
{
	DListRet ret = DLIST_RET_OK;
	DListNode* iter = thiz->first;

	while(iter != NULL)
	{
		print(iter->data);

		iter = iter->next;
	}

	return ret;
}

void dlist_destroy(DList* thiz)
{
	DListNode* iter = thiz->first;
	DListNode* next = NULL;

	while(iter != NULL)
	{
		next = iter->next;
		dlist_node_destroy(iter);
		iter = next;
	}

	thiz->first = NULL;
	free(thiz);

	return;
}

DListRet dlist_foreach(DList* thiz, VisitFunc visitFunc, void* ctx)
{
	DListRet ret = DLIST_RET_OK;
	DListNode* iter = thiz->first;

	while(iter != NULL)
	{
		visitFunc(ctx, iter->data);

		iter = iter->next;
	}

	return ret;
}

