/*
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <iconv.h>
#include <malloc.h>

#define PRINT_ERR() printf("%d:%s\n",__LINE__,strerror(errno))
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
    while(1){
        int skip;
        printf("跳过多少字节？");
        scanf("%d",&skip);
        printf("跳过%d字节\n",skip);
        if(skip){
            int ans;
            in_left=size-skip;
            out_left=CVAL;
            char *p1=p+skip;
            char *o1=&out[0];
            if(iconv(cd,&p1,&in_left,&o1,&out_left)==-1){
                if(errno == E2BIG){
                    fwrite(&out[0],1,CVAL-out_left,stdout);
                } else {
                    status();
                    return 0;
                }
            }
            printf("\n可以吗?");
            scanf("%d",&ans);
            if(ans==skip){
                return skip;
            }
        } else{
            return 0;
        }
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
    dst_rem=DST_BUF_SIZE;
    p1=b1;
    p2=b2;
    size_t sum=0;
    while(1){
        size_t skip;
        if(iconv(cd,&p1,&src_rem,&p2,&dst_rem)==-1){
            size_t rem_size=DST_BUF_SIZE-dst_rem;
            if(rem_size)
            {

                fwrite(b2,rem_size,1,f2);
            }
            switch(errno){
                case E2BIG:
                    sum =total-src_rem;
                    
                    
                    break;
                case EILSEQ:

                    printf("出错了:%ld -> %ld(%ld)\n",sum, total-src_rem,rem_size);
                    skip=correct(cd,p1,src_rem);
                    p1 +=skip;
            }
        }
        p2=b2;
        dst_rem=DST_BUF_SIZE;
    }
    free(b1);
    free(b2);
    fclose(f2);
    return sum;
}

int main(int argc,char **argv)
{
    iconv_t cd=iconv_open("utf-8","gbk");
    size_t errcnt=gbk2utf8(cd,"/home/tec/yttlj-gbk.txt","/home/tec/ytk.txt");
    iconv_close(cd);
    if(errcnt==-1){
        return -1;
    }
    if(errcnt){
        printf("有%ld字符被转换\n",errcnt);
    }
    return 0;
}
