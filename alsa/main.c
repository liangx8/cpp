
/*
     https://blog.51cto.com/buaadallas/403832

 */

#include <unistd.h>
#include <stdio.h>
void usage(void){
    printf("-l 列显所有内容\n-p 参数\n-c 播放标准输入的内容1秒\n-r 录音1秒");
}

void snd_list(void);
void snd_param(void);
void snd_playback(void);
void snd_record(const char *);
int main(int argc, char **argv)
{
    int opt;
    if(argc == 1){
        usage();
        return 0;
    }
    while((opt = getopt(argc,argv,"lpcrd:")) != -1){
        switch (opt)
        {
        case 'l':
            snd_list();
            return 0;
        case 'p':
            snd_param();
            return 0;
        case 'c':
            snd_playback();
            return 0;
        case 'r':
            snd_record(NULL);
            return 0;
        default:
            break;
        }
    }
}
