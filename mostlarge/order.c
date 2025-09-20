#include <wchar.h>
#include <string.h>
#include <malloc.h>
struct LIST{
    int size;
    int num;
    char data[0];
};

/**
 * @brief
 * @param size 每个单位的宽度
 * @param num 总数
 */
void *create_list(int size,int num)
{
    wprintf(L"....%d\n",sizeof(struct LIST));
    return malloc(sizeof(struct LIST)+size*num);
}
int set_at(struct LIST *pl,int pos,void *val){
    if(pos>=pl->num){
        wprintf(L"out of range position:%d\n",pos);
        return -1;
    }
    memcpy(&pl->data[pos * pl->size],val,pl->size);
    return 0;
}
void *get_at(struct LIST *pl,int pos)
{
    if(pos>=pl->num){
        wprintf(L"out of range position:%d\n",pos);
        return NULL;
    }
    return &pl->data[pos];
}
int sort_list(struct LIST *pl,int size)
{
    if(size>pl->num){
        wprintf(L"out of range size:%d\n",size);
        return -1;
    }
    return 0;
}