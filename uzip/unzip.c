/*
文件名是中文时，最多会出现２种编码gbk和utf8,甚至是unicode
这些编码是没办法识别的

文件名称一般是utf8,gbk, unicode很少可能性
*/

#include <wchar.h>
#include <stdio.h>
#include <minizip/unzip.h>
void printInfo(unz_file_info *info)
{
    wprintf(
        L"version           : 0x%x\n"
        L"version_needed    : 0x%x\n"
        L"\033[34mflag\033[0m              : 0x%x\n"
        L"compressed_method : 0x%x\n"
        L"dosDate           : 0x%x\n"
        L"crc               : 0x%x\n"
        L"compressed_size   : %ld\n"
        L"\033[34muncompressed_size\033[0m : %ld\n"
        L"size_filename     : 0x%x\n"
        L"size_file_extra   : 0x%x\n"
        L"size_file_comment : 0x%x\n\n"
        L"disk_num_start    : 0x%x\n"
        L"internal_fa       : 0x%x\n"
        L"external_fa       : 0x%x\n",
        info->version,info->version_needed,info->flag,info->compression_method,info->dosDate,
        info->crc,info->compressed_size,info->uncompressed_size,info->size_filename,info->size_file_extra,
        info->size_file_comment,info->disk_num_start,info->internal_fa,info->internal_fa  
    );
    
}

int extract(const char *name, const char *dstdir)
{
    unzFile *uzh=unzOpen(name);
    if(uzh==NULL){
        
        return -1;
    }
    unzGoToFirstFile(uzh);
    unzClose(uzh);
    return 0;
}
void printsize(uLong size){
    wchar_t uom;
    int sized;
    if(size > (1024 * 1024 * 1024)){
        sized = size / (1024 * 1024 * 1024);
        uom=L'G';
    } else if(size > (1024 * 1024 * 10) ){
        sized = size / (1024 * 1024 );
        uom=L'M';
    } else if(size > 10240){
        sized = size / 1024 ;
        uom=L'K';
    } else {
        sized = size;
        uom=L'B';
    }
    wprintf(L"%5d%lc",sized,uom);

}
void print_date(int year,int month,int mday);
int guess_encode(char *dst,int dst_len,char *src,int src_len);
int dir_proc(const char *path);
int utf8str(char *dst,int dcnt,const char *src);

char buf[2048];
char out1[2048];
char out2[2048];
int content(const char *name)
{
    unzFile uzh=unzOpen(name);
    if(uzh==NULL){
        wprintf(L"文件打开错误%s\n",name);
        return -1;
    }
    unzGoToFirstFile(uzh);
    const int size=sizeof(unz_file_info);
    unz_file_info *info=(unz_file_info *)(&buf[0]);
    int total=0;
    char *fileName=&buf[size];
    char *extraField=&buf[size+128];
    char *comment=&buf[size+128+256];
    while(1){
        unzGetCurrentFileInfo(uzh,
            info,
            fileName,128,
            extraField,256,
            comment,256);
#if 0
        if(info->uncompressed_size==0){
            goto noshow;
        }
        total+=info->uncompressed_size;
        //wprintf(L"%4d-%02d-%02d ",info->tmu_date.tm_year,info->tmu_date.tm_mon,info->tmu_date.tm_mday);
        print_date(info->tmu_date.tm_year,info->tmu_date.tm_mon,info->tmu_date.tm_mday);
        printsize(info->uncompressed_size);
        if(guess_encode(out,4096,fileName,info->size_filename)){
        }else {
            wprintf(L" \033[36m%ls\033[0m\n",(wchar_t *)out);
        }

        //printInfo(info);
        noshow:
#else
        if(guess_encode(out1,2048,fileName,info->size_filename)){
        }else {
            wprintf(L"\033[30;47m%ls\033[0m\n",(wchar_t *)out1);
            utf8str(out2,2048,out1);
            dir_proc(out2);
        }
        printInfo(info);
#endif
        int res=unzGoToNextFile(uzh);
        if(res==UNZ_END_OF_LIST_OF_FILE){
            break;
        }
    }
    unzClose(uzh);
    wprintf(L"Total:");
    printsize(total);
    wprintf(L"\n");
    return 0;
}