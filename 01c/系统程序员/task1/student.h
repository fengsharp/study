#ifndef STUDENT_H_
#define STUDENT_H_

#include "common.h"

typedef struct _Student
{
    char* id;
    char* name;
    unsigned short age;
} Student;

Student* student_create(const char* id, const char* name, unsigned short age);
void student_print(const void* pctx, void* element);
bool student_is_same_id(const void* pctx, const void* element);

void student_destroy(Student* thiz);
void student_destroy_func(void* data);

#endif