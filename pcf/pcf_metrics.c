#include <wchar.h>
#include "pcf_internal.h"

// lsbint32 format;                 /* Always stored with least significant byte first! */
// /* if the format is compressed */
// int16 metrics_count;
// Compressed_Metrics metrics[metrics_count];
// /* else if format is default (uncompressed) */
// int32 metrics_count;
// Uncompressed_Metrics metrics[metrics_count];
// /* endif */


void toc_metrics(long size,long offset,uint32_t format,struct FONT_TOC *font){
    uint8_t buf[4];
    FILE *pcf=font->pcf;
    fseek(pcf,offset,SEEK_SET);
    fread(&buf[0],1,4,pcf);
    toc_comm(buf,size,offset,format,2);
    font->metrics.format=format;
    if(format & PCF_COMPRESSED_METRICS){
        uint8_t buf2[2];
        fread(&buf2[0],1,2,pcf);
        uint32_t cnt;
        if(format & PCF_BYTE_MASK){
            cnt = buf2[0] * 256 + buf2[1];
        } else {
            cnt = *((uint16_t *)&buf2[0]);
        }
        wprintf(L"\tmetrics count:%d\n",cnt);
        font->metrics.count=cnt;
        font->metrics.seek_offsets=offset+2;
    } else {
        wprintf(L"\t not implement\n");
    }
}

// uint8 left_sided_bearing;
// uint8 right_side_bearing;
// uint8 character_width;
// uint8 character_ascent;
// uint8 character_descent;
//  /* Implied character attributes field = 0 */
// Uncompressed

// int16 left_sided_bearing;
// int16 right_side_bearing;
// int16 character_width;
// int16 character_ascent;
// int16 character_descent;
// uint16 character_attributes;

int pcf_metrics(struct FONT_TOC *toc,uint32_t font_idx,uint8_t *buf)
{
    if(toc->metrics.format & PCF_COMPRESSED_METRICS){
        fseek(toc->pcf,toc->metrics.seek_offsets+font_idx*5,SEEK_SET);
        fread(buf,1,5,toc->pcf);
        wprintf(L"left_sided_bearing:%u(%u)\nright_side_bearing:%u(%u)\nwidth:%u(%u),ascent:%u(%u),descent:%u(%u)\n",
            buf[0],0x7f&buf[0],buf[1],0x7f&buf[1],buf[2],0x7f&buf[2],buf[3],0x7f&buf[3],buf[4],0x7f&buf[4]);
    } else {
        wprintf(L"not implements yet\n");
    }
    return 0;
}