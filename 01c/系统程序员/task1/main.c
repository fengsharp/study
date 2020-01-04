#include "common.h"
#include "DataManager.h"
#include "student.h"
#include "dlist.h"

int main(int argc, char* argv[])
{
    const char* const datafile = "data.txt";
    DataManager* pDataMgr = dm_create(datafile);
    RETURN_VALUE_IF_ASSERT_FAIL(pDataMgr != NULL, -1);

    DList* dlist = dlist_create(student_destroy_func);
    RETURN_VALUE_IF_ASSERT_FAIL(dlist != NULL, -1);

    dm_get_all(pDataMgr, dlist);
    dlist_foreach(dlist, NULL, student_print);

    const char* findId = "101";
    const void* pFind = dlist_find(dlist, (void*)findId, student_is_same_id);
    if (pFind)
    {
        puts("find student ok.");
        student_print(NULL, (void*)pFind);
    }

    dlist_destroy(dlist);
    dm_destroy(pDataMgr);

    return 0;
}