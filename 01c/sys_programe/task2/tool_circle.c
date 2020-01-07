#include "tool_circle.h"

#include "common.h"

typedef struct _PrivInfo
{
    char fillColor[8];
} PrivInfo;

static void tool_circle_draw(Tool* thiz, void* ctx)
{
    Circle* pCircle = (Circle*)(ctx);
    PrivInfo* pData = (PrivInfo*)thiz->priv;

    printf("circle draw[mid pt: x=%d, y=%d, radius: %d ] with fill color %s \n", 
            pCircle->midPt.x, pCircle->midPt.y, pCircle->radius, pData->fillColor);
}

static void tool_circle_destroy(Tool* thiz)
{
    SAFE_FREE(thiz);
}

Tool* tool_circle_create()
{
    Tool* pObj = malloc(sizeof(Tool) + sizeof(PrivInfo));
    if (pObj)
    {
        pObj->destroy = tool_circle_destroy;
        pObj->draw = tool_circle_draw;
        memset(pObj->priv, 0, sizeof(PrivInfo));

        PrivInfo* pData = (PrivInfo*)pObj->priv;
        strcpy(pData->fillColor, "blue");
    }
    return pObj;
}