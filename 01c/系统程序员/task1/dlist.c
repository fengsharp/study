#include "dlist.h"

typedef struct _DListNode
{
    struct _DListNode* prev;
    struct _DListNode* next;
    void* data;
} DListNode;

static DListNode* dlist_node_create(void* data)
{
    DListNode* pObj = malloc(sizeof(DListNode));
    RETURN_VALUE_IF_ASSERT_FAIL(pObj != NULL, NULL);
    pObj->prev = NULL;
    pObj->next = NULL;
    pObj->data = data;

    return pObj;
}

static void dlist_node_destroy(DListNode* thiz, DListEleDestroyFunc eleDestroyFunc)
{
    if (eleDestroyFunc && thiz->data)
    {
        eleDestroyFunc(thiz->data);
    }
    SAFE_FREE(thiz);
}

struct _DList
{
    DListNode* pHead;
    DListNode* pTail;
    size_t len;
    DListEleDestroyFunc eleDestroyFunc;
};

DList* dlist_create(DListEleDestroyFunc eleDestroyFunc)
{
    DList* pObj = malloc(sizeof(DList));
    RETURN_VALUE_IF_ASSERT_FAIL(pObj != NULL, NULL);

    pObj->pHead = dlist_node_create(NULL);
    if (!pObj->pHead)
    {
        SAFE_FREE(pObj);
        return NULL;
    }
    pObj->pTail = dlist_node_create(NULL);
    if (!pObj->pTail)
    {
        dlist_node_destroy(pObj->pHead, eleDestroyFunc);
        SAFE_FREE(pObj);
        return NULL;
    }
    pObj->pHead->next = pObj->pTail;
    pObj->pTail->prev = pObj->pHead;
    pObj->len = 0;
    pObj->eleDestroyFunc = eleDestroyFunc;

    return pObj;
}

bool dlist_push_back(DList* thiz, void* data)
{
    DListNode* pNewNode = dlist_node_create(data);
    RETURN_VALUE_IF_ASSERT_FAIL(pNewNode != NULL, false);

    pNewNode->next = thiz->pTail;
    pNewNode->prev = thiz->pTail->prev;

    thiz->pTail->prev->next = pNewNode;
    thiz->pTail->prev = pNewNode;

    ++thiz->len;

    return true;
}

void dlist_foreach(DList* thiz, void* pctx, DListVisitFunc visitFunc)
{
    DListNode* pCurNode = thiz->pHead->next;
    while (pCurNode != thiz->pTail)
    {
        visitFunc(pctx, pCurNode->data);
        pCurNode = pCurNode->next;
    }
}

const void* dlist_find(DList* thiz, void* pctx, DListCompareFunc compareFunc)
{
    DListNode* pCurNode = thiz->pHead->next;
    while (pCurNode != thiz->pTail)
    {
        if (compareFunc(pctx, pCurNode->data))
        {
            return pCurNode->data;
        }
        pCurNode = pCurNode->next;
    }
    return NULL;
}

size_t dlist_get_len(DList* thiz)
{
    return thiz->len;
}

void dlist_destroy(DList* thiz)
{
    DListNode* pCurNode = thiz->pHead;
    while (pCurNode != NULL)
    {
        thiz->pHead = pCurNode->next;
        dlist_node_destroy(pCurNode, thiz->eleDestroyFunc);
        pCurNode = thiz->pHead;
    }
    SAFE_FREE(thiz);
}

