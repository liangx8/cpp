/*
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <iconv.h>
#include <malloc.h>

#define PRINT_ERR() printf("line(%d):%s\n",__LINE__,strerror(errno))
#define DST_BUF_SIZE 102400


void hex(char *p,size_t size)
{
    size_t cnt=0;
    int x=0;
    while(cnt < size){
        printf("%2x",*p & 0xff);
        cnt++;
        p++;
        if(x==15){
            x=0;
            printf("\n");
        }
    }
}

void status(void)
{
    switch(errno){
        case E2BIG:
        printf("There is not sufficient room at out buffer\n");
        break;
        case EILSEQ:
        printf("An invalid multibyte sequence has been encountered in in buffer\n");
        break;
        case EINVAL:
        printf("An incomplete multibyte sequence has been encountered int input\n");
        break;
        default:
        printf(strerror(errno));
        
    }
    
}
#define CVAL  100
size_t correct(iconv_t cd,char *p,size_t size){
    char out[CVAL];
    size_t out_left;
    size_t in_left;
    int skip=1;
    while(1){
        int ans=0;
        in_left=size-skip;
        out_left=CVAL;
        char *p1=p+skip;
        char *o1=&out[0];
        if(iconv(cd,&p1,&in_left,&o1,&out_left)==-1){
            int out_size=CVAL - out_left;
            if(out_size){
                fwrite(&out[0],1,out_size,stdout);
            } else {
                skip++;
                continue;
            }
        }
        printf("\n可以吗?");
        while(ans != 'n'){
            ans=getchar();
            if(ans=='y'){
                return skip;
            }
        }
        skip++;
    }
}

size_t gbk2utf8(iconv_t cd,const char *s1,const char *s2)
{
    size_t total;
    char *b1,*b2,*p1,*p2;
    size_t src_rem,dst_rem;

    FILE *f1=fopen(s1,"r");
    if (f1==NULL){
        PRINT_ERR();
        return -1;
    }
    FILE *f2=fopen(s2,"w+");
    if (f2==NULL){
        PRINT_ERR();
        return -1;
    }

    fseek(f1,0,SEEK_END);
    total=ftell(f1);
    rewind(f1);
    b1=malloc(total);
    b2=malloc(DST_BUF_SIZE);
    fread(b1,1,total,f1);
    if(ferror(f1)){
        PRINT_ERR();
        free(b1);
        return -1;
    }
    fclose(f1);
    src_rem=total;
    p1=b1;
    size_t sum=0;
    while(1){
        size_t skip;
        dst_rem = DST_BUF_SIZE;
        p2=b2;
        if(iconv(cd,&p1,&src_rem,&p2,&dst_rem)==-1){
            size_t rem_size=DST_BUF_SIZE-dst_rem;
            int retcode=errno;
            if(src_rem)
            {
                fwrite(b2,rem_size,1,f2);
            } else {
                break;
            }
            switch(retcode){
                case E2BIG:
                    sum =total-src_rem;
                    break;
                case EILSEQ:
                case EINVAL:

                    printf("出错了:%ld -> %ld(%ld)\n",sum, total-src_rem,rem_size);
                    skip=correct(cd,p1,src_rem);
                    p1 +=skip;
                    src_rem -= skip;
            }
        } else {
            printf("done1 :%lu\n",src_rem);
            break;
        }
    }
    free(b1);
    free(b2);
    fclose(f2);
    return sum;
}

int main(int argc,char **argv)
{
    iconv_t cd=iconv_open("utf-8","gbk");
    if(argc!=3){
        printf("gbk2utf8 <src> <dst>\n");
        return -1;
    }
    size_t errcnt=gbk2utf8(cd,argv[1],argv[2]);
    iconv_close(cd);
    if(errcnt==-1){
        return -1;
    }
    if(errcnt){
        printf("有%ld字符被转换\n",errcnt);
    }
    return 0;
}
