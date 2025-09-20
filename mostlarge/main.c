/**
 * 在指定的目录终查找最大的文件
 */
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <malloc.h>


typedef int (*action)(const char*,const void*,void*);
int walk(const char *,action,void *);
void* act_data_create(int,int);
int act(const char *,const void *,void *);
void show(const void *ent);
int main(int argc, char **argv)
{
    setlocale(LC_ALL,"");
    if (argc == 1)
    {
        const wchar_t *msg=L"usage:\n\tlarge <path>\nfind large file on folder\n";
        wprintf(msg);
        return 0;
    }
    void *ol=act_data_create(10,200);
    int res=walk(argv[1],act,ol);
    show(ol);
    free(ol);
    return res;
}
