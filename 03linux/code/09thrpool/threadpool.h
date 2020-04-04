#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#define WORK_END_FLAG -1

typedef void* (*RunFunc)(void* arg);
typedef struct _Task
{
    RunFunc runCallback;
    void* arg;
    struct _Task* pNext;
} Task;

struct _ThreadPool;
typedef struct _ThreadPool ThreadPool;

ThreadPool* thr_pool_create();
void thr_pool_destroy(ThreadPool* thiz);

void thr_pool_addTask(ThreadPool* thiz, RunFunc runCallback, void* arg);

#endif