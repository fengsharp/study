#include "common.h"

#include "tool_rect.h"
#include "tool_line.h"
#include "tool_circle.h"

int main()
{
    Tool* pTool = tool_rect_create();
    Rect rect = {.lfPt={.x=0, .y=0}, .width=10, .height=20};
    tool_draw(pTool, &rect);
    tool_destroy(pTool);

    pTool = tool_line_create();
    Line line = {.startPt={.x=1, .y=1}, .endPt={.x=99, .y=99}};
    tool_draw(pTool, &line);
    tool_destroy(pTool);

    pTool = tool_circle_create();
    Circle circle = {.midPt={.x=2, .y=2}, .radius=88};
    tool_draw(pTool, &circle);
    tool_destroy(pTool);

    return 0;
}