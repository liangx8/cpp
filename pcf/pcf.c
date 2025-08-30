#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "pcf.h"
void load_char(struct PCF_FONT *pcf,wchar_t ch)
{
    unsigned char glyphName[256];
    unsigned short cidx=encoding_index(pcf,ch);
    if(cidx==0xffff){
        wprintf(L"not available for character [%lc]\n",ch);
    }
    pcf_bitmaps(pcf,cidx,&glyphName[0]);
    pcf_metrics(pcf,cidx,&glyphName[0]);
    glyph_name(pcf,cidx,&glyphName[0],256);
    wprintf(L"font index:%04x->>%s(%lc)\n",cidx,glyphName,ch);

}
typedef struct {
    const char* fontname;
    wchar_t charcode;
} OPTS;
int parse_args(int argc,char **argv,OPTS *opt);
int main(int argc,char **argv)
{
    setlocale(LC_ALL,"");
    OPTS opt;
    parse_args(argc,argv,&opt);
    wprintf(L"load char 0x%04x/%u on font %s\n",opt.charcode,opt.charcode,opt.fontname);

    struct PCF_FONT *pcf=pcf_load(opt.fontname);
    //const wchar_t ch=L'\u33e0';
    load_char(pcf,opt.charcode);
    pcf_release(pcf);
    return 0;
}
