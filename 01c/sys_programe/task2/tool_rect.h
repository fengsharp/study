#ifndef TOOL_RECT_H_
#define TOOL_RECT_H_

#include "tool.h"

typedef struct _Rect
{
    Point lfPt;
    int width;
    int height;
} Rect;

Tool* tool_rect_create();

#endif