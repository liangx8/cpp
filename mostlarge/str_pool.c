#include <malloc.h>
#include <string.h>
#define MAX_SIZE 1024*4
struct STR_POOL{
    int end;
    int unused;
    char data[];
};


void* str_pool_new(void)
{
    struct STR_POOL *pt=malloc(sizeof(struct STR_POOL)+MAX_SIZE);
    pt->end=0;
    return pt;
}
int str_pool_put(struct STR_POOL *lk,const char *str)
{
    int len=strlen(str);
    int after=lk->end + len+1;
    if(after > MAX_SIZE){
        return -1;
    }
    memcpy(&lk->data[lk->end],str,len+1);
    lk->data[after-1]='\0';
    lk->end =after;
    return 0;
}
/**
 * @return 返回的值只在下一次str_pool_put()前有效
 */
const char* str_pool_pop(struct STR_POOL *lk)
{
    int idx=lk->end;
    if(idx==0){
        return NULL;
    }
    idx--;
    // lk->data[idx] should be 0
    while(1){
        idx --;
        if(idx==0){
            lk->end=0;
            break;
        }
        if(lk->data[idx]=='\0'){
            idx++;
            lk->end=idx;
            break;
        }

    }
    return &lk->data[idx];
}