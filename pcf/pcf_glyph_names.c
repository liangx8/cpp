#include <wchar.h>
#include "pcf_internal.h"
// lsbint32 format;                 /* Always stored with least significant byte first! */
// int32 glyph_count;              /* byte ordering depends on format, should be the same as the metrics count */
// int32 offsets[glyph_count];     /* byte offsets to string data */
// int32 string_size;
// char string[string_size];

void toc_glyph_names(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[256];
    FILE *pcf=font->pcf;
    font->glyph_names.format=format;
    fseek(pcf,offset,SEEK_SET);
    fread(&buf[0],2,4,pcf);
    toc_comm(buf,size,offset,format,7);
    uint32_t cnt=0;
    if(format &PCF_BYTE_MASK){
        cnt=msbyte4(&buf[4]);
    } else {
        wprintf(L"\tnot implement\n");
        return;
    }
    font->glyph_names.count=cnt;
    font->glyph_names.seek_offsets=offset+ 4 + 4;
    long data_offset=offset+4 +4 +cnt *4;
    fseek(pcf,data_offset,SEEK_SET);
    fread(&buf[0],1,4,pcf);
    if(format & PCF_BYTE_MASK){
        font->glyph_names.data_size=msbyte4(&buf[0]);
    }else {
        font->glyph_names.data_size=*((uint32_t*)&buf[0]);
    }
    font->glyph_names.seek_data=data_offset+4;
    wprintf(L"\tglyph name:count %d,data size:%ld\n",font->glyph_names.count,font->glyph_names.data_size);
    //wprintf("\toffset:%lx  data:%lx\n",font->glyph_names.seek_offsets,font->glyph_names.seek_data);
}
int glyph_name(struct FONT_TOC *toc,uint32_t index,char *gname,int size)
{
    uint8_t buf[4];
    fseek(toc->pcf,toc->glyph_names.seek_offsets+index*4,SEEK_SET);
    fread(&buf[0],1,4,toc->pcf);
    uint32_t offset;
    if(toc->glyph_names.format & PCF_BYTE_MASK){
        offset=msbyte4(&buf[0]);
    }else {
        offset=*((uint32_t*)&buf[0]);
    }
    fseek(toc->pcf,toc->glyph_names.seek_data+offset,SEEK_SET);
    fread(gname,1,size,toc->pcf);
    return 0;
}
