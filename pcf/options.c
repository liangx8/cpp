#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
typedef struct {
    const char* fontname;
    uint64_t charcode;
} OPTS;
const char *const fontname[]={
"/usr/share/fonts/wenquanyi/wenquanyi_9pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_10pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_11pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_12pt.pcf",
"/usr/share/fonts/wenquanyi/wenquanyi_13px.pcf"
};


/**
 * @brief 10进制，16进制字符串转化成长整型,16进制以0x开始
 */
unsigned long any2num(const char *str)
{
    const char *ptr=str;
    unsigned long base=0;
    while(*ptr==' ')ptr++;
    if(*ptr!='0') goto dec;
    ptr ++;
    if((*ptr!='x') && (*ptr != 'X')) goto dec;
    while(1){
        ptr++;
        int ch=*ptr;
        if(ch=='\0' || ch == ' '){
            return base;
        }
        base = base << 4;
        if((ch >= '0') && (ch <= '9')){
            base += ch - '0';
            continue;
        }
        if((ch >= 'a') && (ch <= 'f')){
            base += ch - 'a' +10;
            continue;
        }
        if((ch >= 'A') && (ch <= 'F')){
            base += ch - 'a' +10;
            continue;
        }
        return 0;
    }
dec:
    while(1){
        int ch=*ptr;
        if(ch=='\0' || ch == ' '){
            return base;
        }
        base = base * 10;
        if(ch >= '0' && ch <= '9'){
            base += ch -'0';
        } else {
            return 0;
        }
        ptr++;
    }
}



int parse_args(int argc,char **argv,OPTS *opt)
{
    int ch;
    int num=4;
    int charcode=0;
    while((ch=getopt(argc,argv,"f:U:"))!=-1){
        switch (ch)
        {
        case 'f':
            num=atoi(optarg);
            break;
        case 'U':
            charcode=any2num(optarg);
            break;
        default:
            break;
        }
    }
    if(num>4)num=4;
    opt->fontname=fontname[num];
    opt->charcode=charcode;

    return 0;
}