/*
pcf format
https://fontforge.org/docs/techref/pcf-format.html
*/

#include <malloc.h>
#include <wchar.h>
#include "pcf_internal.h"



struct _toc_entry{
    uint32_t type;
    uint32_t format;
    uint32_t size;
    uint32_t offset;
};
const char *toc_desc[]={
"PCF_PROPERTIES",
"PCF_ACCELERATORS",
"PCF_METRICS",
"PCF_BITMAPS",
"PCF_INK_METRICS",
"PCF_BDF_ENCODINGS",
"PCF_SWIDTHS",
"PCF_GLYPH_NAMES",
"PCF_BDF_ACCELERATORS"
};
// Properties Table
// struct ProperitsTable{
// uint32_t format;    　             /* Always stored with least significant byte first! */
// uint32_t nprops;                   /* Stored in whatever byte order is specified in the format */
// struct props {
//     uint32_t name_offset;          /* 属性名字的偏移 */
//     uint8_t isStringProp;
//     uint32_t value;                /* 属性的值，或者字符串值的偏移 */
// } props[nprops];
// char padding[(nprops&3)==0?0:(4-(nprops&3))];   /* pad to next int32 boundary */
// int string_size;                /* total size of all strings (including their terminating NULs) */
// char strings[string_size];
// char padding2[];
// };
void toc_comm(uint8_t *pcf,long size,long offset,uint32_t format,int seq){
    wprintf(L"%2d::type:%-21sformat:0x%04x, size:%8ld, offset:0x%06lx\n",seq,toc_desc[seq],format,size,offset);
    uint32_t *pu32=(uint32_t*)pcf;
    if(*pu32 != format){
        wprintf(L"warning : deference TOC format:%x,actual: %x",format,*pu32);
    }
}
void toc_properties(long size,long offset,uint32_t format,struct FONT_TOC *font)
{
    uint8_t *props=malloc(size);
    uint32_t nprop;
    FILE *pcf=font->pcf;
    fseek(pcf,offset,SEEK_SET);
    fread(props,1,size,pcf);
    toc_comm(props,size,offset,format,0);
    unsigned char *ptr=props+4;
    if(format & PCF_BYTE_MASK){
        nprop=msbyte4(ptr);
    } else {
        nprop=*((uint32_t *)ptr);
    }
    ptr +=4;
    int str_offset=(nprop * 9)/ 4 + 3;
    str_offset = str_offset * 4;
    int str_size;
    if(format & PCF_BYTE_MASK){
        str_size=msbyte4(props+str_offset);
    } else {
        str_size=*((uint32_t*)(props+str_offset));
    }
    wprintf(L"\t%d个属性,string占用内存%d字节\n",nprop,str_size);
    uint8_t *str_base=props+str_offset+4;
    for(int ix=0;ix<nprop;ix++){
        uint32_t name_offset,value;
        uint8_t isStringProp=*(ptr+ix*9+4);
        if(format & PCF_BYTE_MASK){
            name_offset=msbyte4(ptr+ix*9);
            value=msbyte4(ptr+ix*9+5);
        }else {
            name_offset=*((uint32_t *)(ptr+ix*9));
            value=*((uint32_t *)(ptr+ix*9+5));
        }
        //wprintf("\t%2d:name offset:%4d,isStringProp:%d,value:%9d/%8x\n",ix+1,name_offset,isStringProp,value,value);
        wprintf(L"\t%s=",str_base+name_offset);
        if(isStringProp){
            wprintf(L"%s\n",str_base + value);
        }else {
            wprintf(L"%d(integer)\n",value);
        }
    }
    free(props);
}
void toc_accelerators(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[4];
    fseek(font->pcf,offset,SEEK_SET);
    fread(&buf[0],1,4,font->pcf);
    toc_comm(buf,size,offset,format,1);
}

void toc_ink_metrics(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[4];
    fseek(font->pcf,offset,SEEK_SET);
    fread(&buf[0],1,4,font->pcf);
    toc_comm(buf,size,offset,format,4);
}

void toc_swidths(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[4];
    fseek(font->pcf,offset,SEEK_SET);
    fread(&buf[0],1,4,font->pcf);
    toc_comm(buf,size,offset,format,6);
}
void toc_bdf_accelerators(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[4];
    fseek(font->pcf,offset,SEEK_SET);
    fread(&buf[0],1,4,font->pcf);
    toc_comm(buf,size,offset,format,8);
}
void toc_bdf_encodings(long size,long offset,uint32_t format,struct FONT_TOC *font);
void toc_glyph_names(long size,long offset,uint32_t format,struct FONT_TOC *font);
void toc_metrics(long size,long offset,uint32_t format,struct FONT_TOC *font);
void toc_bitmaps(long size,long offset,uint32_t format,struct FONT_TOC *font);
const void (*toc_handler[])(long,long,uint32_t,struct FONT_TOC*)={
    toc_properties,
    toc_accelerators,
    toc_metrics,
    toc_bitmaps,
    toc_ink_metrics,
    toc_bdf_encodings,
    toc_swidths,
    toc_glyph_names,
    toc_bdf_accelerators
};
const char magic4[]={'\1','f','c','p'};
int check_magic(char buf[])
{
    for(int ix=0;ix<4;ix++){
        if(buf[ix]!=magic4[ix]){
            return -1;
        }
    }
    return 0;
}
struct FONT_TOC* pcf_load(const char *pcfname)
{
    FILE *pcf=fopen(pcfname,"r");
    if(pcf==NULL){
        wprintf(L"open %s error\n",pcfname);
        return NULL;
    }
    struct FONT_TOC *font=malloc(sizeof(struct FONT_TOC));
    font->pcf=pcf;
    char head[8];
    fread(&head[0],1,8,pcf);
    if(check_magic(head)){
        wprintf(L"warring: 文件的magic char不是 \"\\1fcp\"\n");
    }
    int cnt=*((uint32_t *)&head[4]);
    
    int size=sizeof(struct _toc_entry)*cnt;
    wprintf(L"TOC count:%d\n",cnt);
    struct _toc_entry *toc=malloc(size);
    fread(toc,size,1,pcf);
    for(int ix=0;ix<cnt;ix++){
        for(int ty=0;ty<9;ty++){
            if(toc[ix].type== (1 << ty)){
                toc_handler[ty](toc[ix].size,toc[ix].offset,toc[ix].format,font);
            }
        }
        
    }
    wprintf(L"pos:%lx\n",ftell(pcf));
    free(toc);
    return font;
}
void pcf_release(struct FONT_TOC *toc){
    fclose(toc->pcf);
    free(toc);
}