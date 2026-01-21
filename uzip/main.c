#include <stdio.h>
#include <wchar.h>
#include <locale.h>
int extract(const char *name, const char*dstdir);
int content(const char *name);
int main(int argc,char **argv)
{
    setlocale(LC_ALL,"");
    if(argc==1){
        wprintf(L"usage:\n");
    } else {
        wprintf(L"open %s\n",argv[1]);
        content(argv[1]);
        wprintf(L"done\n");
    }
    return 0;
}
