#ifndef TOOL_H_
#define TOOL_H_

typedef struct _Point
{
    int x;
    int y;
} Point;

struct _Tool;
typedef struct _Tool Tool;

typedef void (*ToolDrawFunc)(Tool* thiz, void* ctx);
typedef void (*ToolDestroy)(Tool* thiz);

struct _Tool
{
    ToolDrawFunc draw;
    ToolDestroy destroy;

    char priv[0];
};

static inline void tool_draw(Tool* thiz, void* ctx)
{
    thiz->draw(thiz, ctx);
}

static inline void tool_destroy(Tool* thiz)
{
    thiz->destroy(thiz);
}

#endif