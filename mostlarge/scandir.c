#include <stdio.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>


static char *fname;
static int nlen;
int filter(const struct dirent* ent)
{
    char type;
    int retval=0;
    switch (ent->d_type){
        case DT_DIR:
        type='D';
        retval=-1;
        break;
        case DT_REG:
        type='R';
        break;
        case DT_LNK:
        type='L';
        break;
        default:
        type='?';
    }
    strcpy(&fname[nlen],ent->d_name);
    printf("%c %s\n",type,fname);
    return retval;
}

void test(const char *name)
{
    struct dirent **nl;
    nlen=strlen(name);
    fname=malloc(nlen+256);
    strcpy(fname,name);
    if(name[nlen-1]!='/'){
        fname[nlen]='/';
        nlen++;
    }
    scandir(name,&nl,filter,NULL);
    free(fname);
}