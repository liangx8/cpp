#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <sqlite3.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
char buf[256];
void foreach(void *sb);
void *read_lines(const char*);
int main(int argc,char **argv)
{
    sqlite3 *db;
    setlocale(LC_ALL,"");
    const char *home=getenv("HOME");
    
    strcpy(buf,home);
    strcat(buf,"/test.db");
    wprintf(L"数据库文件:%s\n",buf);
    int res=sqlite3_open("/home/cyc/test.db",&db);
    if (res==SQLITE_OK)
        wprintf(L"ＳＱＬＩＴＥ３ open result code (%d)\n",res);
    sqlite3_close(db);
    if(argc>1){
        void *obj=read_lines(argv[1]);
        if(obj!=NULL){
            foreach(obj);
            free(obj);
            wprintf(L"释放内存\n");
        }
    }
    return 0;
}

