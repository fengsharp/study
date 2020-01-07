#include "tool_line.h"

#include "common.h"

typedef struct _PrivInfo
{
    char pencil[8];
    int line_width;
} PrivInfo;

static void tool_line_draw(Tool* thiz, void* ctx)
{
    Line* pLine = (Line*)ctx;
    PrivInfo* pData = (PrivInfo*)thiz->priv;

    printf("line draw[start pt: x=%d, y=%d, end pt: x=%d, y=%d] with %s pencil and line width:%d\n", 
            pLine->startPt.x, pLine->startPt.y, pLine->endPt.x, pLine->endPt.y, pData->pencil, pData->line_width);
}

static void tool_line_destroy(Tool* thiz)
{
    SAFE_FREE(thiz);
}

Tool* tool_line_create()
{
    Tool* pObj = malloc(sizeof(Tool) + sizeof(PrivInfo));
    if (pObj)
    {
        pObj->destroy = tool_line_destroy;
        pObj->draw = tool_line_draw;
        memset(pObj->priv, 0, sizeof(PrivInfo));
        PrivInfo* pData = (PrivInfo*)pObj->priv;
        strcpy(pData->pencil, "black");
        pData->line_width = 5;
    }
    return pObj;
}