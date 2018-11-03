#define _BSD_SOURCE
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

typedef char (*file_names_t)[4+FILE_NAME_NUM + 1];

static char file_content[FILE_CONTENT_NUM+1];

static void create_sources();
static void create_content();
static void copy_files();

file_names_t get_file_names(char ch);

static void search1(file_names_t);
static void search2(file_names_t);
static void search3(file_names_t);

static void get_random_uppercase(char* buf, size_t num);
static void get_random_lowercase(char* buf, size_t num);
static void get_random_digits(char* buf, size_t num);

static inline bool has_char(const char *str, char ch);
static inline void print_mode(mode_t mode);

void init(void)
{
    srand(time(NULL));
    create_sources();
    create_content();
    copy_files();

}

void search(char target, int flags)
{
    file_names_t file_names;

    if (chdir("content/") == -1)
    {
        ERROR_EXIT("chdir");
    }

    file_names = get_file_names(target);

    if (flags & OP_1)
    {
        puts("1");
        search1(file_names);
    }
    else
    {
        if (flags & OP_2)
        {
            puts("2");
            search2(file_names);
        }
        if (flags & OP_3)
        {
            puts("3");
            search3(file_names);
        }
    }

    if (chdir("..") == -1)
    {
        ERROR_EXIT("chdir");
    }

    free(file_names);
}

void recursive_remove(const char* name)
{
    struct stat st;
    DIR* dd;
    struct dirent* de;
    bool unlink_flag;

    if (stat(name, &st) == -1)
    {
        ERROR_EXIT("stat");
    }

    if (S_ISDIR(st.st_mode))
    {
        if ((dd = opendir(name)) == NULL)
        {
            ERROR_EXIT("opendir");
        }
        if (chdir(name) == -1)
        {
            ERROR_EXIT("chdir");
        }
        do {
            unlink_flag = false;
            while ((de = readdir(dd)) != NULL)
            {
                if (strcmp("..", de->d_name) == 0 || strcmp(".", de->d_name) == 0)
                    continue;
                if (stat(de->d_name, &st) == -1)
                {
                    ERROR_EXIT("stat");
                }
                if (S_ISDIR(st.st_mode))
                {
                    recursive_remove(de->d_name);
                }
                else
                {
                    if (unlink(de->d_name) == -1)
                    {
                        ERROR_EXIT("unlink");
                    }
                }
                unlink_flag = true;
            }
            if (unlink_flag)
            {
                rewinddir(dd);
            }
        } while (unlink_flag);

        if (closedir(dd) == -1)
        {
            ERROR_EXIT("dd");
        }

        if (chdir("..") == -1)
        {
            ERROR_EXIT("chdir");
        }

        if (rmdir(name) == -1)
        {
            ERROR_EXIT("rmdir");
        }
    }
    else
    {
        if (unlink(name) == -1)
        {
            ERROR_EXIT("unlink");
        }
    }
}

file_names_t get_file_names(char ch)
{
    char path[4];
    DIR *dir;
    struct dirent *file;
    file_names_t file_names;
    size_t file_num;

    if ((file_names = malloc(FILE_NUM * sizeof(*file_names))) == NULL)
    {
        ERROR_EXIT("malloc");
    }
    file_num = 0;

    for (char i = 'A'; i <= 'Z'; ++i)
    {
        for (char j = 'a'; j <= 'z'; ++j)
        {
            path[0] = i;
            path[1] = '/';
            path[2] = j;
            path[3] = '\0';

            if ((dir = opendir(path)) == NULL)
            {
                ERROR_EXIT("opendir");
            }

            while ((file = readdir(dir)) != NULL)
            {
                if (strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0)
                    continue;
                if (has_char(file->d_name, ch))
                {
                    snprintf(file_names[file_num++], sizeof(*file_names),
                             "%s/%s", path, file->d_name);
                }
            }

            if (closedir(dir) == -1)
            {
                ERROR_EXIT("closedir");
            }
        }
    }
    file_names[file_num][0] = '\0';

    return file_names;
}

static void search1(file_names_t file_names)
{
    int fd;
    char new_name[4+8+1];

    for (size_t i = 0; file_names[i][0] != '\0'; ++i)
    {
        if ((fd = open(file_names[i], O_RDWR)) == -1)
        {
            puts(file_names[i]);
            ERROR_EXIT("open ");
        }

        if (ftruncate(fd, 80) == -1)
        {
            ERROR_EXIT("ftruncate");
        }

        if (read(fd, file_content, 80) == -1)
        {
            ERROR_EXIT("read");
        }
        file_content[80] = '\0';
        printf("%lu\nbefore:\n\tpath: content/%s\n\tname: %s\n\tcontent: %s\n",
               i, file_names[i], &file_names[i][4], file_content);

        if (close(fd) == -1)
        {
            ERROR_EXIT("close");
        }

        strncpy(new_name, file_names[i], 4);
        get_random_digits(new_name+4, 8);
        if (rename(file_names[i], new_name) == -1)
        {
            ERROR_EXIT("rename");
        }

        printf("after:\n\tnew name: %s\n", new_name+4);
    }
}

static void search2(file_names_t file_names)
{
    struct stat st;

    for (size_t i = 0; file_names[i][0] != '\0'; ++i)
    {
        if (stat(file_names[i], &st) == -1)
        {
            ERROR_EXIT("stat");
        }

        printf("name: %s\n\told mode: ", file_names[i]+4);

        print_mode(st.st_mode);

        printf("\n\tnew mode: ");

        if (chmod(file_names[i], 0777) == -1)
        {
            ERROR_EXIT("chmod");
        }

        if (stat(file_names[i], &st) == -1)
        {
            ERROR_EXIT("stat");
        }

        print_mode(st.st_mode);
        putchar('\n');
    }
}

static void search3(file_names_t file_names)
{
    struct stat st;

    for (size_t i = 0; file_names[i][0] != '\0'; ++i)
    {
        if (stat(file_names[i], &st) == -1)
        {
            ERROR_EXIT("stat");
        }
        printf("name: %s\nbefore:\n\tuser id: %u\n\tgroup id: %u\n", file_names[i]+4, st.st_uid, st.st_gid);

        if (chown(file_names[i], st.st_uid+1, st.st_gid+1) == -1)
        {
            ERROR_EXIT("chown");
        }

        if (stat(file_names[i], &st) == -1)
        {
            ERROR_EXIT("stat");
        }

        printf("after:\n\tuser id: %u\n\tgroup id: %u\n", st.st_uid, st.st_gid);
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
            get_random_uppercase(file_name, FILE_NAME_NUM);
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
        // puts(src_file_name);
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
    for (size_t i = 0; i < num; ++i)
    {
        buf[i] = (rand() % 26) + 'A';
    }
    buf[num] = '\0';
}

void get_random_lowercase(char* buf, size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        buf[i] = (rand() % 26) + 'a';
    }
    buf[num] = '\0';
}

void get_random_digits(char* buf, size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        buf[i] = (rand() % 10) + '0';
    }
    buf[num] = '\0';
}

void print_mode(mode_t mode)
{
    if (mode & S_IRUSR)
        putchar('r');
    else
        putchar ('-');

    if (mode & S_IWUSR)
        putchar('w');
    else
        putchar('-');

    if (mode & S_IXUSR)
        putchar('x');
    else
        putchar('-');

    // group permission
    if (mode & S_IRGRP)
        putchar('r');
    else
        putchar ('-');

    if (mode & S_IWGRP)
        putchar('w');
    else
        putchar('-');

    if (mode & S_IXGRP)
        putchar('x');
    else
        putchar('-');

    // other permission
    if (mode & S_IROTH)
        putchar('r');
    else
        putchar ('-');

    if (mode & S_IWOTH)
        putchar('w');
    else
        putchar('-');

    if (mode & S_IXOTH)
        putchar('x');
    else
        putchar('-');
}
