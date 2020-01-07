#include "tool_rect.h"

#include "common.h"

typedef struct _PrivInfo
{
    char pencil[8];
} PrivInfo;

static void tool_rect_draw(Tool* thiz, void* ctx)
{
    Rect* rect = (Rect*)(ctx);
    PrivInfo* pData = (PrivInfo*)thiz->priv;

    printf("rect draw[start pt: x=%d, y=%d, widht=%d, height=%d] with %s pencil\n", rect->lfPt.x, rect->lfPt.y, rect->width, rect->height, pData->pencil);
}

static void tool_rect_destroy(Tool* thiz)
{
    SAFE_FREE(thiz);
}

Tool* tool_rect_create()
{
    Tool* pObj = malloc(sizeof(Tool) + sizeof(PrivInfo));
    if (pObj)
    {
        pObj->destroy = tool_rect_destroy;
        pObj->draw = tool_rect_draw;
        memset(pObj->priv, 0, sizeof(PrivInfo));
        PrivInfo* pData = (PrivInfo*)pObj->priv;
        strcpy(pData->pencil, "red");
    }
    return pObj;
}