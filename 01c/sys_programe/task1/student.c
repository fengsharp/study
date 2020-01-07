#include "student.h"

#include "common.h"

Student* student_create(const char* id, const char* name, unsigned short age)
{
    Student* pObj = malloc(sizeof(Student));
    RETURN_VALUE_IF_ASSERT_FAIL(pObj != NULL, NULL);

    memset(pObj, 0, sizeof(Student));
    pObj->id = strdup(id);
    pObj->name = strdup(name);
    pObj->age = age;

    return pObj;
}

void student_print(const void* pctx, void* element)
{
    Student* pStu = (Student*)(element);
    printf("%s\t%s\t%hu\n", pStu->id, pStu->name, pStu->age);
}

bool student_is_same_id(const void* pctx, const void* element)
{
    Student* pStu = (Student*)(element);
    char* pId = (char*)(pctx);
    return (strlen(pId) == strlen(pStu->id)) &&
           (strcmp(pId, pStu->id) == 0);
}

void student_destroy(Student* thiz)
{
    SAFE_FREE(thiz->id);
    SAFE_FREE(thiz->name);
    SAFE_FREE(thiz);
}

void student_destroy_func(void* data)
{
    Student* pStu = (Student*)(data);
    student_destroy(pStu);
}