#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <malloc.h>
int on_error(void *dummy,int code)
{
    printf("error: %s\n",strerror(code));
    return -1;
}
struct Entry{
    char *name;
    size_t size;
};
struct Entry *result;
int size;
void init(int sz)
{
    result=malloc(sizeof(struct Entry)*sz);
    bzero(result,sizeof(struct Entry)*sz);
    size=sz;
}
void add(const char *name,size_t sz)
{
    int newlen=strlen(name);
    if(sz > result[0].size){
        char *holder;
        if(result[0].name == NULL){
            holder=malloc(newlen+1);
        }else{
            holder=result[0].name;
            int oldlen=strlen(holder);
            if(oldlen<newlen){
                holder=realloc(holder,newlen+1);
            }
        }
        strcpy(holder,name);
        int is_most=1;
        for(int ix=1;ix<size;ix++){
            if(result[ix].size < sz){
                result[ix-1].name=result[ix].name;
                result[ix-1].size=result[ix].size;
            } else {
                // 将当前值填如ix-1位置
                result[ix-1].name=holder;
                result[ix-1].size=sz;
                is_most=0;
                break;
            }
        }
        if(is_most){
            result[size-1].name=holder;
            result[size-1].size=sz;
        }
    }
}
int onefile(const char *name)
{
    struct stat st;
    if(stat(name,&st)){
        printf("stat error on '%s'\n",name);
        on_error(NULL,errno);
        return -1;
    }
    if((st.st_mode & S_IFMT)==S_IFREG){
        add(name,st.st_size);
    }
    return 0;
}
#define K  1024
#define M  (K * K)
#define G  (M * K)
void most(void){
    for(int ix=0;ix<size;ix++){
        if(result[size-ix-1].name==NULL){
            printf("%2d:\n",ix);
        } else {
            size_t sz=result[size-ix-1].size;
            char uom=' ';
            if(sz > M){
                sz=sz / M;
                uom='M';
            }else if (sz > K){
                sz = sz /K;
                uom='K';
            }
            printf("%2d: %5ld%c %s\n",ix,sz,uom,result[size-ix-1].name);
        }
    }
}
