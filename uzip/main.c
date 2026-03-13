#include <stdio.h>
#include <wchar.h>
#include <locale.h>
//#include <unistd.h>
//const char *uname="国际标";
void test_dir(void);
int init_iconv(void);
int extract(const char *name, const char*dstdir);
int content(const char *name);
int main(int argc,char **argv)
{
    if(init_iconv()){
        return -1;
    }
    setlocale(LC_ALL,"");
    if(argc==1){
        wprintf(L"usage:wchar_t %ld\n",sizeof(wchar_t));
    } else {
        wprintf(L"open %s\n",argv[1]);
        if(content(argv[1])){
            test_dir();
        }
        wprintf(L"done\n");
    }
    return 0;
}
