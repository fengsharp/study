#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "array_list.h"
#include "common.h"

typedef struct _Task
{
    int no;
    char* name;
} Task;

Task* task_create(int index)
{
    Task* pTask = (Task*)malloc(sizeof(Task));
    pTask->no = index;

    char buf[32];
    snprintf(buf, sizeof(buf), "task%d", index);
    pTask->name = strdup(buf);

    return pTask;
}

void task_destroy(void* ctx)
{
    Task* pObj = (Task*)ctx;
    SAFE_FREE(pObj->name);
    SAFE_FREE(pObj);
}

static int task_cmp(void* ctx, void* element)
{
    Task* pDataCtx = (Task*)(ctx);
    Task* pDataEle = (Task*)(element);

    if (pDataCtx->no == pDataEle->no)
        return 0;
    else 
        return pDataCtx->no > pDataEle->no ? 1 : -1;
}

static void test_array_list_append()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    const int fixedCapacity = 10;
    for (int i = 0; i < fixedCapacity; ++i)
    {
        bool ret = array_list_append(pArr, task_create(i));
        assert(ret == true);
    }
    assert(array_list_size(pArr) == fixedCapacity);
    assert(array_list_capacity(pArr) == fixedCapacity);

    assert(array_list_append(pArr, task_create(fixedCapacity)) == true);
    assert(array_list_size(pArr) == (fixedCapacity+1));
    assert(array_list_capacity(pArr) != fixedCapacity);

    unsigned int newCapacity = array_list_capacity(pArr);
    unsigned int newSize = fixedCapacity + (fixedCapacity >> 1);
    assert(newCapacity == newSize);

    for (int i=0; i<=fixedCapacity; ++i)
    {
        Task* pTask = (Task*)array_list_value(pArr, i);
        assert(pTask->no == i);
    }
    
    array_list_destroy(pArr);
}

static void test_array_list_prepend()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    const int fixedCapacity = 10;
    for (int i = 0; i < fixedCapacity; ++i)
    {
        bool ret = array_list_prepend(pArr, task_create(i));
        assert(ret == true);
    }
    assert(array_list_size(pArr) == fixedCapacity);
    assert(array_list_capacity(pArr) == fixedCapacity);

    array_list_prepend(pArr, task_create(fixedCapacity));
    assert(array_list_size(pArr) == (fixedCapacity+1));
    assert(array_list_capacity(pArr) != fixedCapacity);
    unsigned int newCapacity = array_list_capacity(pArr);
    unsigned int newSize = fixedCapacity + (fixedCapacity >> 1);
    assert(newCapacity == newSize);

    for (int i=0; i<=fixedCapacity; ++i)
    {
        Task* pTask = (Task*)array_list_value(pArr, fixedCapacity-i);
        assert(pTask->no == i);
    }
    
    array_list_destroy(pArr);
}

static void test_array_list_insert()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    Task* pTask = task_create(0);
    assert(pTask);
    assert(array_list_insert(pArr, 1, pTask) == false);
    assert(array_list_insert(pArr, 0, NULL) == false);
    assert(array_list_insert(pArr, 0, pTask) == true);
    assert(array_list_insert(pArr, 1, task_create(2)) == true);
    assert(array_list_insert(pArr, 1, task_create(1)) == true);

    assert(((Task*)array_list_value(pArr, 0))->no == 0);
    assert(((Task*)array_list_value(pArr, 1))->no == 1);
    assert(((Task*)array_list_value(pArr, 2))->no == 2);

    Task* pTask4 = task_create(4);
    assert(pTask4);
    assert(array_list_insert(pArr, 4, pTask4) == false);
    assert(array_list_insert(pArr, 3, task_create(3)) == true);
    assert(array_list_insert(pArr, 4, pTask4) == true);

    assert(array_list_insert(pArr, 0, task_create(100)) == true);
    assert(array_list_size(pArr) == 6);

    array_list_destroy(pArr);
}

static void test_array_list_delete()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    array_list_delete(pArr, 0);
    assert(array_list_size(pArr) == 0);

    const int fixedCapacity = 10;
    const int appendCount = fixedCapacity + (fixedCapacity >> 1);
    for (int i = 0; i < appendCount; ++i)
    {
        bool ret = array_list_append(pArr, task_create(i));
        assert(ret == true);
    }
    assert(array_list_size(pArr) == appendCount);
    assert(array_list_append(pArr, task_create(appendCount)) == true);
    unsigned int nowCapacity = array_list_capacity(pArr);
    unsigned int newCapacity = appendCount + (appendCount >> 1);
    assert(nowCapacity == newCapacity);

    newCapacity = newCapacity >> 2;
    int tmpCount = array_list_size(pArr)-newCapacity;
    for (int i=0; i<tmpCount; ++i)
    {
        array_list_delete(pArr, array_list_size(pArr)-1);
    }
    assert(array_list_capacity(pArr) == newCapacity * 2);

    for (int i=0; i<array_list_size(pArr); ++i)
    {
        Task* pTask = (Task*)array_list_value(pArr, i);
        assert(pTask->no == i);
    }
    
    array_list_destroy(pArr);
}

static void test_array_list_delete_range()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    const int fixedCount = 10;
    for (int i = 0; i < fixedCount; ++i)
    {
        bool ret = array_list_append(pArr, task_create(i));
        assert(ret == true);
    }
    assert(array_list_size(pArr) == fixedCount);
    array_list_delete_range(pArr, fixedCount/2, fixedCount-fixedCount/2);
    assert(array_list_size(pArr) == fixedCount/2);
 
    for (int i=0; i<array_list_size(pArr); ++i)
    {
        Task* pTask = (Task*)array_list_value(pArr, i);
        assert(pTask->no == i);
    }
    
    array_list_destroy(pArr);
}

static void test_array_list_index_of()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    const int fixedCount = 10;
    for (int i = 0; i < fixedCount; ++i)
    {
        bool ret = array_list_append(pArr, task_create(i));
        assert(ret == true);
    }
    assert(array_list_size(pArr) == fixedCount);
 
    for (int i=0; i<array_list_size(pArr); ++i)
    {
        Task task = {.no = i, .name=NULL};
        assert(array_list_index_of(pArr, &task, task_cmp) == i);
    }
    
    array_list_destroy(pArr);
}

static void test_array_list_clear()
{
    ArrayList* pArr = array_list_create(task_destroy);
    assert(pArr != NULL);
    assert(array_list_size(pArr) == 0);
    assert(array_list_value(pArr, 0) == 0);

    const int fixedCount = random() % 50 + 10;
    for (int i = 0; i < fixedCount; ++i)
    {
        bool ret = array_list_append(pArr, task_create(i));
        assert(ret == true);
    }
    assert(array_list_size(pArr) == fixedCount);
    
    array_list_clear(pArr);

    assert(array_list_size(pArr) == 0);
    assert(array_list_capacity(pArr) >= 10 && array_list_capacity(pArr) <= 20);
    
    array_list_destroy(pArr);
}

int main()
{
    test_array_list_append();
    test_array_list_prepend();
    test_array_list_insert();

    test_array_list_delete();
    test_array_list_delete_range();

    test_array_list_index_of();
    test_array_list_clear();

    puts("=== all ok. ===");
    return 0;
}