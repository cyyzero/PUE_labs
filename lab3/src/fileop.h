#ifndef FILEOP_H
#define FILEOP_H

#define FILE_NAME_NUM      5
#define FILE_CONTENT_NUM 100
#define FILE_NUM         100

#include <stdio.h>

#define ERROR_EXIT(str)                                    \
do {                                                       \
    fprintf(stderr, "%s %d\n", __FILE__, __LINE__);        \
    perror(str);                                           \
    exit(-1);                                              \
} while (0)                                                \

void init(void);

#endif
