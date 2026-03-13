#include <stdio.h>
#include <wchar.h>
#include <malloc.h>
struct StringBuffer{
    void       *buf;
    size_t     size;
    const char **strs;
    int        strs_size;
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
    fread(sb->buf,1,size,hdl);
    fclose(hdl);
    return base;
}
