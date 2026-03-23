#include <stdio.h>
#include <wchar.h>
#include <malloc.h>
#include <ctype.h>
struct StringBuffer{
    void        *buf;
    long         size;
    const char **strs;
    int          strs_size;
};
void *read_lines(const char *fname)
{
    FILE *hdl=fopen(fname,"r");
    if(hdl==NULL){
        return NULL;
    }
    fseek(hdl,0,SEEK_END);
    long size=ftell(hdl)+1;
    wprintf(L"size:%ld\n",size);
    rewind(hdl);
    int max_linesize=size/4;
    void *base=malloc(sizeof(struct StringBuffer)+size+max_linesize * sizeof(void *));
    if(base==NULL){
        fclose(hdl);
        return NULL;
    }
    struct StringBuffer *sb=base;
    sb->buf=base+sizeof(struct StringBuffer);
    sb->size=size;
    sb->strs=base+size+sizeof(struct StringBuffer);
    fread(sb->buf,1,size,hdl);
    fclose(hdl);
    *(sb->strs)=sb->buf;
    char *sptr;
    const char **ptrs=sb->strs+1;
    int cnt=1;
    int str_begin=0;
    for(int ix=0;ix<size;ix++){
        sptr=sb->buf+ix;
        if(*sptr=='\n' || *sptr=='\r'){
            *sptr='\0';
            str_begin=1;
            continue;
        }
        if(str_begin){
            str_begin=0;
            *ptrs=sptr;
            ptrs++;
            cnt++;
        }
    }
    sptr=sb->buf+size-1;
    *sptr='\0';
    // sb->buf[size]='\0';
    sb->strs_size=cnt;
    return base;
}
int blankline(const char *str);
void foreach(struct StringBuffer *sb)
{
    int cnt=0;

    while(cnt < sb->strs_size){
        if(blankline(*(sb->strs+cnt))){
            wprintf(L"空行\n");
        }else{
            wprintf(L"%3d %s\n",cnt,*(sb->strs+cnt));
        }
        cnt ++;
    }
}
/**
 * @brief 检查是否是纯空行
 * @param str 
 * @return 0非空行 1空行
 */
int blankline(const char *str)
{
    while(*str){
        if(isblank(*str)){
            str++;
        } else {
            return 0;
        }
    }
    return -1;
}