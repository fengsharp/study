#include "DataManager.h"

#include <string.h>

#include "common.h"
#include "student.h"
#include "dlist.h"

struct _DataManager
{
    char* datafile;

    FILE* pFile;
};

static bool db_connect(DataManager* thiz)
{
    RETURN_VALUE_IF_ASSERT_FAIL(thiz->pFile == NULL, true);

    thiz->pFile = fopen(thiz->datafile, "r");
    return thiz->pFile != NULL;
}

static void db_close(DataManager* thiz)
{
    fclose(thiz->pFile);
    thiz->pFile = NULL;
}

DataManager* dm_create(const char* datafile)
{
    DataManager* pObj = malloc(sizeof(DataManager));
    RETURN_VALUE_IF_ASSERT_FAIL(pObj != NULL, NULL);

    pObj->datafile = strdup(datafile);
    pObj->pFile = NULL;
    
    return pObj;
}

void dm_destroy(DataManager* thiz)
{
    SAFE_FREE(thiz->datafile);
    SAFE_FREE(thiz);
}


void dm_get_all(DataManager* thiz, DList* dlist)
{
    if (!db_connect(thiz))
    {
        return;
    }

    char line_id[8] = { '\0' };
    char line_name[16] = { '\0' };
    unsigned short line_age = 0;
    int ret = EOF;
    // fscanf 非常不安全，既不能保证line_id末尾填入尾0，也不能对容器长度做检查
    while ((ret = fscanf(thiz->pFile, "%s %s %hu", line_id, line_name, &line_age)) != EOF)
    {
        Student* pStu = student_create(line_id, line_name, line_age);
        if (pStu != NULL)
        {
            dlist_push_back(dlist, pStu);
        }
    }

    db_close(thiz);
}