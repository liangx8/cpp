#include <wchar.h>
#include <sys/stat.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "order.h"

struct ENTRY{
    char *name;
    off_t size;
};


int ent_cmp(const struct ENTRY *l,const struct ENTRY *r)
{
    unsigned long ll=l->size;
    unsigned long lr=r->size;
    if(ll>lr) return -1;
    if(ll<lr) return 1;
    return 0;
    //return (int)(long)(ll-lr);
}

struct{
    int idx;
    int num;
    int cap;
} mgr;
const int bs=sizeof(struct ENTRY);
void* act_data_create(int num,int cap)
{
    mgr.idx=0;
    mgr.num=num;
    mgr.cap=cap;
    return malloc(cap*sizeof(struct ENTRY));
}
const char xxx[]="/home/com/iso/ffx_2us";
int act(const char *name,const struct stat *st,void *ol)
{
#if 1
    struct ENTRY *pt=(ol+mgr.idx*bs);
    int equal=1;
    for(int ix=0;ix<16;ix++){
        if(name[ix]!=xxx[ix]){
            equal=0;
        }
    }
    if(equal){
        wprintf(L"%ld,%s\n",st->st_size,name);
    }
    pt->name=strdup(name);
    pt->size=st->st_size;
    mgr.idx++;
    if(mgr.idx==mgr.cap){
        qsort(ol,mgr.cap,bs,(int (*)(const void*,const void*))(ent_cmp));
        mgr.idx=mgr.num;
        for(int ix=mgr.num;ix<mgr.cap;ix++){
            pt=(ol+ix*bs);
            free(pt->name);
        }
    }
#else
    wprintf(L"file:\033[35m%s\033[0m\n",name);
#endif
    return 0;
}
#define K 1024
#define M (K * K)
#define G (M * K)
void show(const struct ENTRY *ent)
{

    for(int ix=0;ix<mgr.num;ix++){
        wchar_t uom=L'b';
        off_t size=ent->size;
        if(size > (off_t)10*G){
            uom=L'G';
            size=size/G;

        } else if(size > M){
            uom = L'M';
            size=size/M;
        } else if(size >K){
            uom = L'K';
            size=size/K;
        }
        wprintf(L"%2d %5d%lc %s\n",ix,size,uom,ent->name);
        ent++;
    }
}