#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#define PRINT_ERR() printf("%s(%d):%s\n", __FILE__,__LINE__, strerror(errno))
int walkdir(const char * name,int (*act)(const char*), int *(err_act)(struct dirent *, int))
{
    DIR *dir;
    char *fname;
    int nlen=strlen(name);
    fname=malloc(nlen+256);
    strcpy(fname,name);
    if(fname[nlen-1]!='/'){
        fname[nlen]='/';
        nlen++;
    }
    dir=opendir(name);
    if (dir == NULL)
    {
        printf("%s ",name);
        PRINT_ERR();
        return -1;
    }
    else
    {
        // work on readdir()
        while(1){
            errno = 0;
            struct dirent *de=readdir(dir);
            if(de == NULL){
                if(errno){
                    if(err_act(de,errno)){
                        closedir(dir);
                        free(fname);
                        return -1;
                    }
                    continue;
                }
                break;
            }
            if(de->d_name[0]=='.'){
                if((de->d_name[1]==0) || de->d_name[1]=='.'){
                    continue;
                }
            }
            strcpy(&fname[nlen],de->d_name);
            switch (de->d_type)
            {
            case DT_DIR:
                if(walkdir(fname,act,err_act)){
                    closedir(dir);
                    free(fname);
                    return -1;
                }
            break;
            case DT_LNK:
            case DT_REG:
                if(act(fname)){
                    closedir(dir);
                    free(fname);
                    return -1;
                }
            break;
            case DT_CHR:
            case DT_FIFO:
            case DT_SOCK:
            case DT_UNKNOWN:
            default:
                break;
            }
        }
        closedir(dir);
        free(fname);
        return 0;
    }

}
int walk(const char *top, int (*act)(const char*), int *(err_act)(struct dirent *, int))
{
    struct stat st;
    if (stat(top, &st))
    {
        PRINT_ERR();
        return -1;
    }
    switch (st.st_mode & S_IFMT)
    {
    case S_IFBLK:
    case S_IFCHR:
    case S_IFIFO:
        break;
    case S_IFDIR:
        walkdir(top,act,err_act);
        break;
    case S_IFREG:
        act(top);
    }
    return 0;
}
