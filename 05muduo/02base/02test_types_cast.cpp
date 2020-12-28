#include <stdio.h>

#include "Types.h"

class Top
{
public:
    virtual ~Top()
    {
    }
};

class Bottom : public Top
{
};

int main()
{
    double d = 3.14;
    int i = implicit_cast<double>(d);
    printf("%d\n", i);

    Top * pTop = new Bottom;
    Bottom * pBottom = down_cast<Bottom *>(pTop);
    (void)pBottom;

    return 0;
}