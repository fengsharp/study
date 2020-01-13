#include "array_list.h"

#include "common.h"

typedef void* DataType;
struct _ArrayList
{
    ArryListDataFreeFunc dataFreeFunc;

    DataType* data;
    unsigned int size;
    unsigned int capacity;
};

static const unsigned int FIXED_MIN_CAPACITY = 10;
ArrayList* array_list_create(ArryListDataFreeFunc dataFreeFunc)
{
    ArrayList* pObj = (ArrayList*)(malloc(sizeof(ArrayList)));
    RETURN_VALUE_IF_ASSERT_FAIL(pObj != NULL, NULL);
    memset(pObj, 0, sizeof(ArrayList));

    pObj->dataFreeFunc = dataFreeFunc;
    pObj->data = NULL;
    pObj->size = 0;
    pObj->capacity = 0;
    return pObj;
}

void array_list_destroy(ArrayList* thiz)
{
    if(thiz != NULL)
	{
        if (thiz->dataFreeFunc)
        {
            for(int i = 0; i < thiz->size; i++)
            {
                thiz->dataFreeFunc(thiz->data[i]);
            }
        }
		
		SAFE_FREE(thiz->data);
		SAFE_FREE(thiz);
	}
}

static bool private_array_list_enlarge(ArrayList* thiz)
{
    if (thiz->size + 1 > thiz->capacity)
    {
        unsigned int newSize = 0;
        if (thiz->capacity > 0)
        {
            newSize = thiz->capacity + (thiz->capacity >> 1);
        }
        else
        {
            newSize = FIXED_MIN_CAPACITY;
        }
                
        DataType* pNewData = realloc(thiz->data, sizeof(DataType) * newSize);
        RETURN_VALUE_IF_ASSERT_FAIL(pNewData != NULL, false);
        thiz->data = pNewData;
        thiz->capacity = newSize;
    }
    return true;
}

static void private_array_list_shrink(ArrayList* thiz)
{
    unsigned int half_capacity = (thiz->capacity >> 2);
    if (thiz->capacity > FIXED_MIN_CAPACITY && thiz->size <= half_capacity)
    {
        unsigned int new_size = thiz->size << 1;
        new_size = MAX(new_size, FIXED_MIN_CAPACITY);
        DataType* data = (DataType*)realloc(thiz->data, sizeof(DataType) * new_size);
        if (data != NULL)
        {
            thiz->data = data;
            thiz->capacity = new_size;
        }
    }
}

bool array_list_insert(ArrayList* thiz, unsigned int index, void* data)
{
    RETURN_VALUE_IF_FAIL(thiz != NULL && index <= thiz->size && data != NULL, false);
    if (private_array_list_enlarge(thiz) == false)
    {
        return false;
    }

    // void * memmove(void *dst, const void *src, size_t len);
    memmove(&(thiz->data[index+1]), &(thiz->data[index]), (thiz->size - index) * sizeof(DataType));

    thiz->data[index] = data;
    ++thiz->size;
    return true;
}

bool array_list_append(ArrayList* thiz, void* data)
{
    return array_list_insert(thiz, thiz->size, data);
}

bool array_list_prepend(ArrayList* thiz, void* data)
{
    return array_list_insert(thiz, 0, data);
}

void array_list_delete_range(ArrayList* thiz, unsigned int index, unsigned int len)
{
    if (index >= thiz->size || (index + len) > thiz->size)
    {
        return;
    }

    if (thiz->dataFreeFunc)
    {
        for (int i=index; i<(index+len); ++i)
        {
            thiz->dataFreeFunc(thiz->data[i]);
        }
    }

    memmove(&thiz->data[index], &thiz->data[index + len], (thiz->size - (index + len)) * sizeof(DataType));
    thiz->size -= len;

    private_array_list_shrink(thiz);
}

void array_list_delete(ArrayList* thiz, unsigned int index)
{
    array_list_delete_range(thiz, index, 1);
}

int array_list_index_of(ArrayList* thiz, void* data, ArryListDataCmpFunc cmpFunc)
{
    for (int i=0; i<thiz->size; ++i)
    {
        if (cmpFunc(data, thiz->data[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void array_list_clear(ArrayList* thiz)
{
    array_list_delete_range(thiz, 0, thiz->size);
}

unsigned int array_list_size(ArrayList* thiz)
{
    return thiz->size;
}

unsigned int array_list_capacity(ArrayList* thiz)
{
    return thiz->capacity;
}

void* array_list_value(ArrayList* thiz, unsigned int index)
{
    if (index < thiz->size)
    {
        return thiz->data[index];
    }
    else
    {
        return NULL;
    }
}