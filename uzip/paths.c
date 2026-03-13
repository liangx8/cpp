#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include <stdio.h>
#include <libgen.h> //dirname(),basename()
void test_dir(void)
{
    if(mkdir("dir1",0777)){
        wprintf(L"错误:%s\n",strerror(errno));
        return;
    }

}
char pbuf[2048];
int dir_proc(const char *path)
{
    strcpy(pbuf,path);
    char *dn=dirname(pbuf);
    wprintf(L"\033[36m%s\033[0m\n",dn);
    strcpy(pbuf,path);
    char *bn=basename(pbuf);
    wprintf(L"\033[35m%s\033[0m\n",bn);
    return 0;
}
