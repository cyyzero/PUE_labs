#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define FILE_NAME_LEN 5

void get_random_file_name(char *name);
void print(char *name);
bool check_limits();
void fork_and_exec();
void err_exit(const char*s);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (check_limits())
    {
        fork_and_exec();
    }
}

void get_random_file_name(char *name)
{
    for (int i = 0; i < FILE_NAME_LEN; ++i)
    {
        name[i] = rand() % 26 + 'A';
    }
    name[FILE_NAME_LEN] = '.';
    name[FILE_NAME_LEN+1] = 'c';
    name[FILE_NAME_LEN+2] = 0;
}

void print(char *name)
{
    FILE *fp;
    int fd;

    for (;;)
    {
        get_random_file_name(name);
        fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0664);
        if (fd == -1)
        {
            if (errno == EEXIST)
                continue;
            else
                err_exit("open");
        }
        else
            break;
    }
    fp = fdopen(fd, "w");

    const char *s = "#include <stdio.h>%c#include <errno.h>%c#include <stdbool.h>%c#include <stdlib.h>%c#include <unistd.h>%c#include <sys/types.h>%c#include <sys/stat.h>%c#include <sys/wait.h>%c#include <fcntl.h>%c#include <time.h>%c%c#define FILE_NAME_LEN 5%c%cvoid get_random_file_name(char *name);%cvoid print(char *name);%cbool check_limits();%cvoid fork_and_exec();%cvoid err_exit(const char*s);%c%cint main(int argc, char *argv[])%c{%c    srand(time(NULL));%c%c    if (check_limits())%c    {%c        fork_and_exec();%c    }%c}%c%cvoid get_random_file_name(char *name)%c{%c    for (int i = 0; i < FILE_NAME_LEN; ++i)%c    {%c        name[i] = rand() %% 26 + 'A';%c    }%c    name[FILE_NAME_LEN] = '.';%c    name[FILE_NAME_LEN+1] = 'c';%c    name[FILE_NAME_LEN+2] = 0;%c}%c%cvoid print(char *name)%c{%c    FILE *fp;%c    int fd;%c%c    for (;;)%c    {%c        get_random_file_name(name);%c        fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0664);%c        if (fd == -1)%c        {%c            if (errno == EEXIST)%c                continue;%c            else%c                err_exit(%copen%c);%c        }%c        else%c            break;%c    }%c    fp = fdopen(fd, %cw%c);%c%c    const char *s = %c%s%c;%c    fprintf(fp, s, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 34, s, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 34, 34, 34, 10, 10, 34, 34, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 34, 34, 10, 10, 34, 34, 10, 34, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10);%c%c    fclose(fp);%c}%c%cbool check_limits()%c{%c    FILE *fp;%c    int cnt;%c    bool ret;%c%c    fp = fopen(%climits.conf%c, %cr%c);%c    if (fp == NULL)%c        err_exit(%cfopen%c);%c%c    fscanf(fp, %c%%d%c, &cnt);%c    fclose(fp);%c    if (cnt > 0)%c    {%c        ret = true;%c        fp = fopen(%climits.conf%c, %cw%c);%c        if (fp == NULL)%c            err_exit(%cfopen%c);%c        fprintf(fp, %c%%d%c, cnt-1);%c        fclose(fp);%c    }%c    else%c    {%c        ret = false;%c    }%c%c    return ret;%c}%c%cvoid fork_and_exec()%c{%c    char name[FILE_NAME_LEN+3];%c    char command[100];%c    int status;%c%c    print(name);%c    sprintf(command, %cgcc -Wall %%s -o %%.*s%c, name, FILE_NAME_LEN, name);%c    system(command);%c    switch (fork())%c    {%c    case -1:%c        err_exit(%cfork%c);%c%c    case 0:%c        name[FILE_NAME_LEN] = 0;%c        if (execl(name, name, NULL) == -1)%c            err_exit(%cexecl%c);%c%c    default:%c        wait(&status);%c        _exit(0);%c    }%c}%c%cvoid err_exit(const char*s)%c{%c    perror(s);%c    exit(1);%c}%c";
    fprintf(fp, s, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 34, s, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 34, 34, 34, 10, 10, 34, 34, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 34, 34, 10, 10, 34, 34, 10, 34, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10);

    fclose(fp);
}

bool check_limits()
{
    FILE *fp;
    int cnt;
    bool ret;

    fp = fopen("limits.conf", "r");
    if (fp == NULL)
        err_exit("fopen");

    fscanf(fp, "%d", &cnt);
    fclose(fp);
    if (cnt > 0)
    {
        ret = true;
        fp = fopen("limits.conf", "w");
        if (fp == NULL)
            err_exit("fopen");
        fprintf(fp, "%d", cnt-1);
        fclose(fp);
    }
    else
    {
        ret = false;
    }

    return ret;
}

void fork_and_exec()
{
    char name[FILE_NAME_LEN+3];
    char command[100];
    int status;

    print(name);
    sprintf(command, "gcc -Wall %s -o %.*s", name, FILE_NAME_LEN, name);
    system(command);
    switch (fork())
    {
    case -1:
        err_exit("fork");

    case 0:
        name[FILE_NAME_LEN] = 0;
        if (execl(name, name, NULL) == -1)
            err_exit("execl");

    default:
        wait(&status);
        _exit(0);
    }
}

void err_exit(const char*s)
{
    perror(s);
    exit(1);
}