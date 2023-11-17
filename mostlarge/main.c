/**
 * 在指定的目录终查找最大的文件
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
// #include <malloc.h>
#include <sys/stat.h>
#include <dirent.h>

#define PRINT_ERR() printf("%s(%d):%s\n",__FILE__, __LINE__, strerror(errno))
#define DST_BUF_SIZE 102400

int walk(const char *top, int (*act)(const char*), int (*err_act)(struct dirent *, int));
int on_error(struct dirent *ent, int code);
int onefile(const char *);
void init(size_t size);
void most(void);
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        const char *msg="usage:\n\tlarge <path>\nfind large file on folder\n";
        printf(msg);
        return 0;
    }
    init(10);
    walk(argv[1], onefile, on_error);
    most();
    return 0;
}
