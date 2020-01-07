#ifndef TOOL_LINE_H_
#define TOOL_LINE_H_

#include "tool.h"

typedef struct _Line
{
    Point startPt;
    Point endPt;
} Line;

Tool* tool_line_create();

#endif