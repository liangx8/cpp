#include <iconv.h>
#include <wchar.h>
#include <stdio.h>
iconv_t utf8;
iconv_t gbk;
iconv_t unicode;
int init_iconv(void)
{
    utf8=iconv_open("WCHAR_T","UTF8");
    if(utf8==(iconv_t)-1){
        wprintf(L"iconv_open():UTF8 => UNICODE error\n");
        return -1;
    }
    gbk=iconv_open("WCHAR_T","GBK");
    if(gbk==(iconv_t)-1){
        wprintf(L"iconv_open():GBK => UNICODE error\n");
        return -1;
    }
    unicode=iconv_open("UTF8","WCHAR_T");
    if(unicode==(iconv_t)-1){
        wprintf(L"iconv_open():GBK => UNICODE error\n");
        return -1;
    }
    return 0;
}

int utf8str(char *dst,int dcnt,char *src){
    char *pdst=dst;
    size_t dlen=dcnt;
    char *psrc=src;
    const wchar_t *wsrc=(const wchar_t *)src;
    size_t slen=0;
    while(*wsrc){
        slen++;
        wsrc++;
    }
    slen = slen * sizeof(wchar_t);
    iconv(unicode,&psrc,&slen,&pdst,&dlen);
    if(slen != 0){
        // not expected
        perror("panic");
    }
    int len=dcnt-dlen;
    dst[len+1]='\0';
    return len;
}
/**
 * @brief ...
 * @param dst ...
 * @param dst_len size 
 */
int guess_encode(char *dst,int dst_len,char *src,int src_len){

    char *pdst=dst;
    size_t dlen=dst_len;
    char *psrc=src;
    size_t slen=src_len;
    iconv(utf8,&psrc,&slen,&pdst,&dlen);
    if(slen==0){
        *((wchar_t *)pdst)=0;
        return 0;
    }
    pdst=dst;
    dlen=dst_len;
    psrc=src;
    slen=src_len;
    iconv(gbk,&psrc,&slen,&pdst,&dlen);
    if(slen==0){
        *((wchar_t *)pdst)=0;
        return 0;
    }
    wprintf(L" 字符串无法识别\n");
    return -1;
}