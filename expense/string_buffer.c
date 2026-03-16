#include <stdio.h>
#include <wchar.h>
#include <malloc.h>
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
    long size=ftell(hdl);
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
    char *sptr=sb->buf;
    const char **ptrs=sb->strs+1;
    for(int ix=0;ix<size;ix++){
        int str_begin=0;
        while(1){
            if(*sptr=='\n' || *sptr=='\r'){
                *sptr='\0';
                sptr++;
                str_begin=1;
                continue;
            }
    }
    return base;
}
