#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "pcf.h"

const char *const fontname[]={
"/usr/share/fonts/wenquanyi/wenquanyi_10pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_11pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_12pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_13px.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_9pt.pcf"};
int main(int argc,char **argv)
{
    const char *name;
    unsigned char glyphName[256];
    setlocale(LC_ALL,"");
    if(argc>1){
        switch(argv[1][0]){
            case '1':
                name=fontname[0];
                break;
            case '2':
            name=fontname[1];
            break;
        case '3':
            name=fontname[2];
            break;
        case '4':
            name=fontname[3];
            break;
        default:
        name=fontname[4];
        break;
    }
    } else {
        name=fontname[3];
    }
    struct PCF_FONT *pcf=pcf_load(name);
    //const wchar_t ch=L'\u33e0';
    const wchar_t ch=L'二';
    wprintf(L"read font %s\n",name);
    unsigned short cidx=encoding_index(pcf,ch);
    if(cidx==0xffff){
        wprintf(L"not available for character [%lc]\n",ch);
        pcf_release(pcf);
        return 0;
    }
    pcf_bitmaps(pcf,cidx,&glyphName[0]);
    pcf_metrics(pcf,cidx,&glyphName[0]);
    glyph_name(pcf,cidx,&glyphName[0],256);
    pcf_release(pcf);
    wprintf(L"font index:%04x->>%s(%lc)\n",cidx,glyphName,ch);
    return 0;
}
