#include <stdio.h>
#include <wchar.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include "order.h"

struct ENTRY{
    const char *name;
    off_t size;
};


static char *fname;
int filter(const struct dirent* ent)
{
    switch (ent->d_type){
        case DT_DIR:
        if(ent->d_name[0]!='.'){
            return -1;
        }
        if(ent->d_name[1]=='\0'){
            break;
        }
        if(ent->d_name[1]!='.'){
            return -1;
        }
        break;
        case DT_REG:
        break;
        case DT_LNK:
        break;
        default:
    }
    return 0;
}

void test(const char *name)
{
    struct dirent **nl;
    int nlen=strlen(name);
    fname=malloc(nlen+256);
    strcpy(fname,name);
    if(fname[nlen-1]!='/'){
        fname[nlen]='/';
        fname[nlen+1]='\0';
    }
    wprintf(L"扫描目录:%s\n",fname);
    void *lk=str_pool_new();
    str_pool_put(lk,fname);
    while(1){
        const char *dir=str_pool_pop(lk);
        if(dir==NULL){
            break;
        }
        const int dirlen=strlen(dir);
        int total=scandir(dir,&nl,filter,NULL);
        if(total==-1){
            wprintf(L"scandir() error %s\n",dir);
            return;
        }
        for(int ix =0; ix<total;ix++){
            strcpy(fname,dir);
            strcpy(&fname[dirlen],nl[ix]->d_name);
            int len=strlen(fname);
            fname[len]='/';
            fname[len+1]='\0';
            wprintf(L"namelist(%2d):%s\n",ix,fname);
            if(str_pool_put(lk,fname)){
                wprintf(L"内存不够，无法处理文件%s\n",fname);
            }
            free(nl[ix]);
        }
        free(nl);
    }
    free_link(lk);
    free(fname);
}