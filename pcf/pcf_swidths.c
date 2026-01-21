#include <wchar.h>
#include "pcf_internal.h"

int pcf_swidths(struct FONT_TOC *toc,uint32_t font_idx)
{
    long offset=font_idx*4+toc->swidths+8;
    unsigned char buf4[4];
    
    fseek(toc->pcf,offset,SEEK_SET);
    fread(buf4,1,4,toc->pcf);
    int val=msbyte4(buf4);
    wprintf(L" swidth:%d\n",val);
    return val;
}