#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <sqlite3.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
char buf[256];
int foreach(void *sb,typeof(int (*)(int,const char*)));
int onestr(int seq,const char *str)
{
    wprintf(L"%2d %s\n",seq,str);
    return 0;
}
void *read_lines(const char*);
int main(int argc,char **argv)
{
    sqlite3 *db;
    setlocale(LC_ALL,"");
    const char *home=getenv("HOME");
    
    strcpy(buf,home);
    strcat(buf,"/test.db");
    wprintf(L"数据库文件:%s\n",buf);
    int res=sqlite3_open(buf,&db);
    if (res==SQLITE_OK)
        wprintf(L"ＳＱＬＩＴＥ３ open result code (%d)\n",res);
    sqlite3_close(db);
    const char *textname;
    if(argc>1){
        textname=argv[1];
    } else {
        strcpy(buf,home);
        strcat(buf,"/git/linux/txt/big-expense.txt");
        textname=buf;
    }
    void *obj=read_lines(textname);
    if(obj!=NULL){
        foreach(obj,onestr);
        free(obj);
        wprintf(L"释放内存\n");
    }
    return 0;
}

