#include <wchar.h>
#include "pcf_internal.h"

// lsbint32 format;                /* Always stored with least significant byte first! */
// int16 min_char_or_byte2;        /* As in XFontStruct */
// int16 max_char_or_byte2;        /* As in XFontStruct */
// int16 min_byte1;                /* As in XFontStruct */
// int16 max_byte1;                /* As in XFontStruct */
// int16 default_char;             /* As in XFontStruct */
// int16 glyphindeces[(max_char_or_byte2-min_char_or_byte2+1)*(max_byte1-min_byte1+1)];
//                                 /* Gives the glyph index that corresponds to each encoding value */
//                                 /* a value of 0xffff means no glyph for that encoding */


// man xfontstruct可看到XFontStruct的结构描述
//            unsigned min_char_or_byte2;   /* first character */
//            unsigned max_char_or_byte2;   /* last character */
//            unsigned min_byte1;           /* first row that exists */
//            unsigned max_byte1;           /* last row that exists */
//            unsigned default_char;        /* char to print for undefined character */
// 这几个字段基本可以忽略

void toc_bdf_encodings(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[256];
    fseek(font->pcf,offset,SEEK_SET);
    fread(&buf[0],64,4,font->pcf);
    toc_comm(buf,size,offset,format,5);
    font->bdf_encodings.format=format;
    font->bdf_encodings.seek_offset=offset+14;
    font->bdf_encodings.default_char=*((int16_t*)&buf[12]);
    uint16_t *p16=(uint16_t*)&buf[4];
    wprintf(L"\tmin_char_or_byte2:%4x\n",*p16++>>8);
    wprintf(L"\tmax_char_or_byte2:%4x\n",*p16++>>8);
    wprintf(L"\tmin_byte1:        %4x\n",*p16++>>8);
    wprintf(L"\tmin_byte2:        %4x\n",*p16++>>8);
    wprintf(L"\tdefault_char:     %4x\n",*p16++>>8);
}
uint16_t encoding_index(struct FONT_TOC *font,uint16_t ucs)
{
    uint8_t buf[2];
    long offset=ucs * 2 + font->bdf_encodings.seek_offset;
    fseek(font->pcf,offset,SEEK_SET);
    fread(&buf[0],1,2,font->pcf);
    if(*((uint16_t*)&buf[0])==0xffff){
        return 0xffff;
    }
    if(font->bdf_encodings.format & PCF_BYTE_MASK){
        return buf[0]*256+buf[1];
    } else {
        return buf[1]*256+buf[0];
    }
}
