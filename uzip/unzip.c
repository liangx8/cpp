#include <wchar.h>
#include <stdio.h>
#include <minizip/unzip.h>
void printInfo(unz_file_info *info)
{
    wprintf(
        L"version           : 0x%x\n"
        L"version_needed    : 0x%x\n"
        L"flag              : 0x%x\n"
        L"compressed_method : 0x%x\n"
        L"dosDate           : 0x%x\n"
        L"crc               : 0x%x\n"
        L"compressed_size   : %ld\n"
        L"uncompressed_size : %ld\n"
        L"size_filename     : 0x%x\n"
        L"size_file_extra   : 0x%x\n"
        L"size_file_comment : 0x%x\n\n"
        L"disk_num_start    : 0x%x\n"
        L"internal_fa       : 0x%x\n"
        L"external_fa       : 0x%x\n\n",
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

char buf[2048];
int content(const char *name)
{
    unzFile uzh=unzOpen(name);
    if(uzh==NULL){
        
        return -1;
    }
    unzGoToFirstFile(uzh);
    const int size=sizeof(unz_file_info);
    unz_file_info *info=(unz_file_info *)(&buf[0]);
    int total=0;
    while(1){
        unzGetCurrentFileInfo(uzh,
            info,
            &buf[size],128,
            &buf[size+128],256,
            &buf[size+128+256],256);
        total+=info->uncompressed_size;
        wprintf(L"%4d-%02d-%02d ",info->tmu_date.tm_year,info->tmu_date.tm_mon,info->tmu_date.tm_mday);
        printsize(info->uncompressed_size);
        wprintf(L" \033[36;1m%s\033[0m\n",&buf[size]);
        printInfo(info);
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