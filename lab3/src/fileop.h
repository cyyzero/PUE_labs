#ifndef FILEOP_H
#define FILEOP_H

#define FILE_NAME_NUM      5
#define FILE_CONTENT_NUM 100
#define FILE_NUM         1000

#define OP_1 0x1
#define OP_2 0x2
#define OP_3 0x4

#define ERROR_EXIT(str)                                    \
do {                                                       \
    fprintf(stderr, "%s %d\n", __FILE__, __LINE__);        \
    perror(str);                                           \
    exit(-1);                                              \
} while (0)                                                \

void init(void);
void search(char target, int flags);
void recursive_remove(const char* name);

#endif
