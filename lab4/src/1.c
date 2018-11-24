#include <stdio.h>

void print(const char *name)
{
    FILE *fp = fopen(name, "w+");
    const char *s = "#include <stdio.h>%c%cvoid print(const char *name)%c{%c    FILE *fp = fopen(name, %cw+%c);%c    const char *s = %c%s%c;%c    fprintf(fp, s, 10, 10, 10, 10, 34, 34, 10, 34, s, 34, 10, 10, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10);%c    fclose(fp);%c}%c%cint main()%c{%c    print(%ctest.c%c);%c    return 0;%c}%c";
    fprintf(fp, s, 10, 10, 10, 10, 34, 34, 10, 34, s, 34, 10, 10, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10);
    fclose(fp);
}

int main()
{
    print("test.c");
    return 0;
}
