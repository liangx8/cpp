#include <wchar.h>
#include "pcf_internal.h"

// lsbint32 format;                /* Always stored with least significant byte first! */
// int32 glyph_count;              /* byte ordering depends on format, should be the same as the metrics count */
// int32 offsets[glyph_count];     /* byte offsets to bitmap data */
// int32 bitmapSizes[4];           /* the size the bitmap data will take up depending on various padding options */
//                                 /*  which one is actually used in the file is given by (format&3) */
// char bitmap_data[bitmapsizes[format&3]];    /* the bitmap data. format contains flags that indicate: */
//                                 /* the byte order (format&4 => LSByte first)*/
//                                 /* the bit order (format&8 => LSBit first) */
//                                 /* how each row in each glyph's bitmap is padded (format&3) */
//                                 /*  0=>bytes, 1=>shorts, 2=>ints */
//                                 /* what the bits are stored in (bytes, shorts, ints) (format>>4)&3 */
//                                 /*  0=>bytes, 1=>shorts, 2=>ints */

void toc_bitmaps(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[256];
    FILE *pcf=font->pcf;
    font->bitmaps.format=format;
    fseek(pcf,offset,SEEK_SET);
    fread(&buf[0],8,4,pcf);
    toc_comm(buf,size,offset,format,3);
    int32_t glyph_count;
    if(format & PCF_BYTE_MASK){
        glyph_count = msbyte4(&buf[4]);
    } else {
        glyph_count = *((int32_t*)(&buf[4]));
    }
    font->bitmaps.count=glyph_count;
    font->bitmaps.seek_offsets=offset+8;
    long pos=offset+8+glyph_count*4;
    wprintf(L"\tglyph count:%d format: %2x\n",glyph_count,format);
    fseek(pcf,pos,SEEK_SET);
    fread(&buf[0],4,4,pcf);
    for(int ix=0;ix<4;ix++){
        wprintf(L"\t\tbitmapSizes%d:%06x\n",ix,msbyte4(&buf[ix*4]));
    }
    font->bitmaps.seek_data=pos+16;
}

int pcf_bitmaps(struct FONT_TOC *font,long font_index,uint8_t *buf)
{
    int32_t idx_offset=font->bitmaps.seek_offsets;
    int32_t data_offset=font->bitmaps.seek_data;
    fseek(font->pcf,idx_offset+font_index*4,SEEK_SET);
    fread(buf,8,32,font->pcf);
    long dot=msbyte4(buf);
    wprintf(L"offset:%d,%d\n",dot,dot+data_offset);
    fseek(font->pcf,data_offset+dot,SEEK_SET);
    fread(buf,8,32,font->pcf);
    wprintf(L"　０１２３４５６７８９ＡＢＣＤＥＦ\n");
    for(int ix=0;ix<16;ix++){
        wprintf(L"%02d",ix);
        long x=msbyte4(&buf[ix*4]);
        for(int ix=0;ix<32;ix++){
            if(x & 0x80000000){
                wprintf(L"\U0001f3fb");
            }else {
                wprintf(L"\uff3f");
            }
            x = x<<1;
        }
        wprintf(L"\n");
    }
    wprintf(L"\n");
    return 0;
}