#ifndef TOOL_CIRCLE_H_
#define TOOL_CIRCLE_H_

#include "tool.h"

typedef struct _Circle
{
    Point midPt;
    int radius;
} Circle;

Tool* tool_circle_create();

#endif