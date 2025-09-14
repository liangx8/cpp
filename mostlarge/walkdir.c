#include <wchar.h>
#include <sys/stat.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include "order.h"

typedef int(* action)(const char *,const struct stat *);

int put_it(void *sp,const char *name)
{
    int len=strlen(name);
    char fname[512];
    memcpy(fname,name,len);
    if(fname[len-1]=='/'){
        fname[len]='\0';
    } else {
        fname[len]='/';
        fname[len+1]='\0';
    }
    wprintf(L"%s\n",fname);
    if(str_pool_put(sp,fname)){
        wprintf(L"内存不够\n");
        return -1;
    }
    return 0;
}
int _walk_any(const char *name,action act,void *sp)
{
    struct stat st;
    if(stat(name,&st)){
        return -1;
    }
    switch (st.st_mode & S_IFMT)
    {
    case S_IFBLK:
    case S_IFCHR:
    case S_IFIFO:
        break;
    case S_IFDIR:
        if(put_it(sp,name)){
            return -1;
        }
        break;
    case S_IFREG:
        return act(name,&st);
    }
    return 0;
}
int _walk_dir(const char *name,action act,void *sp)
{
    DIR *dir=opendir(name);
    if(dir==NULL){
        wprintf(L"打开目录错误%s\n",name);
        return -1;
    }
    char fname[256];
    while(1){
        struct dirent *de=readdir(dir);
        if(de==NULL){
            break;
        }
        const char *pt=de->d_name;
        if(pt[0]=='.'){
            if((pt[1]=='\0')||(pt[1]=='.')){
                continue;
            }
        }
        strcpy(fname,name);
        strcat(fname,pt);
        switch (de->d_type)
        {
        case DT_DIR:
            put_it(sp,fname);
            break;
        case DT_REG:
        case DT_UNKNOWN:
            _walk_any(fname,act,sp);
            break;
        default:
            break;
        }
    }
    closedir(dir);
    return 0;
}
/**
 * @brief walkthrough a directory recusrely with undefine order
 */
int walk(const char *parent,action act)
{
    void *sp=str_pool_new();
    if(_walk_any(parent,act,sp)){
        goto err_exit;
    }
    char name[256];
    while(1){
        const char *dir=str_pool_pop(sp);
        if(dir==NULL){
            break;
        }
        strncpy(name,dir,255);
        wprintf(L"扫描目录%s\n",name);
        if(_walk_dir(name,act,sp)){
            goto err_exit;
        }
    }
    free_link(sp);
    return 0;
err_exit:
    free_link(sp);
    return -1;
}