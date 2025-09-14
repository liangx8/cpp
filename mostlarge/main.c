/**
 * 在指定的目录终查找最大的文件
 */
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <sys/stat.h>
#include "order.h"

int walk(const char *,typeof(int (*)(const char *,struct stat*)));
int act(const char *name,struct stat *st)
{
    return 0;
}
int main(int argc, char **argv)
{
    setlocale(LC_ALL,"");
    if (argc == 1)
    {
        const wchar_t *msg=L"usage:\n\tlarge <path>\nfind large file on folder\n";
        wprintf(msg);
        void *ol=create_list(0,0);
        free_list(ol);
        return 0;
    }
    return walk(argv[1],act);
}
