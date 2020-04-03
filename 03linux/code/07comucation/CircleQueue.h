#ifndef CIRCLE_QUEUE_H_
#define CIRCLE_QUEUE_H_

struct _CircleQueue;
typedef struct _CircleQueue CircleQueue;

CircleQueue* cir_queue_create();
void cir_queue_destroy(CircleQueue* thiz);

void cir_queue_enqueue(CircleQueue* thiz, void* pData);
void* cir_queue_dequeue(CircleQueue* thiz);

#endif