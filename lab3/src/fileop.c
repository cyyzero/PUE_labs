#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fileop.h"

static char file_content[FILE_CONTENT_NUM+1];

static void create_sources();
static void create_content();
static void copy_files();
// static void find1();
// static void find2();
// static void find3();
static void get_random_uppercase(char* buf, size_t num);
static void get_random_lowercase(char* buf, size_t num);
static void get_random_digits(char* buf, size_t num);
static inline bool has_char(const char *str, char ch);

void init(void)
{
    srand(time(NULL));
    create_sources();
    create_content();
    copy_files();
}

static void find1(char ch)
{
    static char path[1024];
    static char path2[1024];

    DIR *dir;
    struct dirent *file;
    int fd;

    getcwd(path, 1024);
    if ((dir = opendir("sources")) == NULL)
    {
        ERROR_EXIT("opendir");
    }

    while ((file = readdir(dir)) != NULL)
    {
        if (has_char(file->d_name, ch))
        {
            size_t len = strlen(path);
            strncat(path, file->d_name, FILE_NAME_NUM);
            if ((fd = open(path, O_RDONLY)) == -1)
            {
                ERROR_EXIT("open");
            }
            switch (read(fd, file_content, FILE_CONTENT_NUM))
            {
            case -1:
                ERROR_EXIT("read");
            case FILE_CONTENT_NUM:
                break;
            default:
                fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }
            printf("before: path: %s\tname: %s\tcontent: %s\n", path, file->d_name, file_content);
            strncpy(path2, path, len+1);
            get_random_digits(path2+len, 8);
            if (rename(path, path2) == -1)
            {
                ERROR_EXIT("rename");
            }
            printf("afte:  name:  %s", path2+len);

        }
    }
    if (closedir(dir) == -1)
    {
        ERROR_EXIT("closedir");
    }
}

static void find2()
{
    DIR *dir;
    struct dirent *file;

    if ((dir = opendir("sources")) == NULL)
    {
        ERROR_EXIT("opendir");
    }

    while ((file = readdir(dir)) != NULL)
    {
        
    }
    if (closedir(dir) == -1)
    {
        ERROR_EXIT("closedir");
    }
}

static void find3()
{
    DIR *dir;
    struct dirent *file;

    if ((dir = opendir("sources")) == NULL)
    {
        ERROR_EXIT("opendir");
    }

    while ((file = readdir(dir)) != NULL)
    {
        
    }
    if (closedir(dir) == -1)
    {
        ERROR_EXIT("closedir");
    }
}

void create_sources()
{
    char file_name[FILE_NAME_NUM + 1];
    // char file_content[FILE_CONTENT_NUM];
    int fd;

    if (mkdir("sources", 0744) == -1)
    {
        ERROR_EXIT("mkdir");
    }
    if (chdir("sources") == -1)
    {
        ERROR_EXIT("chdir");
    }

    for (int i = 0; i < FILE_NUM; ++i)
    {
        for (;;)
        {
            get_random_uppercase(file_name, FILE_NAME_NUM + 1);
            if ((fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1)
            {
                if (errno == EEXIST)
                    continue;
                else
                    ERROR_EXIT("open");
            }
            else
                break;
        }
        get_random_lowercase(file_content, FILE_CONTENT_NUM);
        switch (write(fd, file_content, FILE_CONTENT_NUM))
        {
        case -1:
            ERROR_EXIT("write");
        case FILE_CONTENT_NUM:
            break;
        default:
            fprintf(stderr, "write error");
            exit(EXIT_FAILURE);
        }

        if (close(fd) == -1)
        {
            ERROR_EXIT("close");
        }
    }

    if (chdir("../") == -1)
    {
        ERROR_EXIT("chdir");
    }
}

void create_content()
{
    if (mkdir("content", 0744) == -1)
    {
        ERROR_EXIT("mkdir");
    }

    if (chdir("./content") == -1)
    {
        ERROR_EXIT("chdir");
    }

    for (char i = 'A'; i <= 'Z'; ++i)
    {
        char dir_name[4];

        dir_name[0] = i;
        dir_name[1] = '\0';
        if (mkdir(dir_name, 0744) == -1)
        {
            ERROR_EXIT("mkdir");
        }
        for (char j = 'a'; j <= 'z'; ++j)
        {
            dir_name[1] = '/';
            dir_name[2] = j;
            dir_name[3] = '\0';
            if (mkdir(dir_name, 0744) == -1)
            {
                ERROR_EXIT("mkdir");
            }
        }
    }
    if (chdir("../") == -1)
    {
        ERROR_EXIT("chdir");
    }
}

static void copy_files()
{
    const size_t LEN = 8;
    char src_file_name[LEN + FILE_NAME_NUM + 1];
    char dest_file_name[LEN + 5 + FILE_NAME_NUM];
    DIR *dir;
    struct dirent *file;
    int src_fd, dest_fd;

    if ((dir = opendir("sources")) == NULL)
    {
        ERROR_EXIT("opendir");
    }

    while ((file = readdir(dir)) != NULL)
    {
        if (strcmp("..", file->d_name) == 0 || strcmp(".", file->d_name) == 0)
            continue;
        snprintf(src_file_name, sizeof(src_file_name), "sources/%s", file->d_name);
        puts(src_file_name);
        if ((src_fd = open(src_file_name, O_RDONLY)) == -1)
        {
            ERROR_EXIT("open");
        }
        switch (read(src_fd, file_content, FILE_CONTENT_NUM))
        {
        case -1:
            ERROR_EXIT("read");
        case FILE_CONTENT_NUM:
            break;
        default:
            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }

        snprintf(dest_file_name, sizeof(dest_file_name), "content/%c/%c/%s", src_file_name[LEN],
                 file_content[0], file->d_name);

        if ((dest_fd = open(dest_file_name, O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1)
        {
            ERROR_EXIT("open");
        }
        switch (write(dest_fd, file_content, FILE_CONTENT_NUM))
        {
        case -1:
            ERROR_EXIT("write");
        case FILE_CONTENT_NUM:
            break;
        default:
            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }

        if (close(dest_fd) == -1)
            ERROR_EXIT("close");
        if (close(src_fd) == -1)
            ERROR_EXIT("close");
    }

    if (closedir(dir) == -1)
    {
        ERROR_EXIT("closedir");
    }
}

// str must be null terminated
bool has_char(const char *str, char ch)
{
    while (*str)
    {
        if (ch == *str)
        {
            return true;
        }
        else
        {
            ++str;
        }
    }
    return false;
}

void get_random_uppercase(char* buf, size_t num)
{
    for (size_t i = 0; i < num-1; ++i)
    {
        buf[i] = (rand() % 26) + 'A';
    }
    buf[num] = '\0';
}

void get_random_lowercase(char* buf, size_t num)
{
    for (size_t i = 0; i < num-1; ++i)
    {
        buf[i] = (rand() % 26) + 'a';
    }
    buf[num] = '\0';
}

void get_random_digits(char* buf, size_t num)
{
    for (size_t i = 0; i < num-1; ++i)
    {
        buf[i] = (rand() % 10) + '0';
    }
    buf[num] = '\0';
}
