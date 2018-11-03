#include "fileop.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{

    if (strcmp(argv[1], "init") == 0)
    {
        init();
    }
    else if (strcmp(argv[1], "del") == 0)
    {
        recursive_remove("content");
        recursive_remove("sources");
    }
    else if (strcmp(argv[1], "search") == 0)
    {
        int flags = 0;
        char target = argv[2][0];

        for (const char *p = argv[3]; *p; ++p)
        {
            if (*p == '1')
            {
                flags |= OP_1;
            }
            if (*p == '2')
            {
                flags |= OP_2;
            }
            if (*p == '3')
            {
                flags |= OP_3;
            }
        }
        search(target, flags);
    }


    return 0;
}
