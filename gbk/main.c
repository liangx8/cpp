/*
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <iconv.h>
#include <malloc.h>

#define SRC_SIZE 2048
#define DST_SIZE (size_t)(SRC_SIZE * 1.5)

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

void status(size_t cnt,size_t in_left,size_t out_left)
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
    printf("transfer count: %ld, error: %d, in: %ld, out: %ld\n",cnt,errno,in_left,out_left);
    
}
#define CVAL  40
size_t correct(iconv_t cd,char *p,size_t rest){
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
            in_left=rest-skip;
            out_left=CVAL;
            char *p1=p+skip;
            char *o1=out;
            iconv(cd,&p1,&in_left,&o1,&out_left);
            fwrite(out,CVAL-out_left,1,stdout);
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
int gbk2utf8(iconv_t cd,FILE *f1,FILE *f2)
{
    int retcode;
    size_t dst_left,src_left=0;
    size_t src_size=SRC_SIZE;

    char *b1,*b2,*p1,*p2;
    
    b1=malloc(SRC_SIZE);
    b2=malloc(DST_SIZE);
    while(!feof(f1)){
        size_t t_cnt;
        dst_left=DST_SIZE;
        p1=b1;
        p2=b2;
        src_size=fread(b1+src_left,1,src_size,f1);
        src_left += src_size;
        t_cnt=iconv(cd,&p1,&src_left,&p2,&dst_left);
        if(t_cnt==-1){
            switch(errno){
            case E2BIG:
                // unexpect error, quit program
                printf("输出缓存不够\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                retcode=-1;
                goto err_exit;
            case EILSEQ:// 无效的字符编码,跳过一字节
                fwrite(b2,1,DST_SIZE-dst_left,stdout);
                status(t_cnt,src_left,dst_left);
                {
                    size_t cr=correct(cd,p1,src_left);
                    if(cr==0) return -1;
                    src_left-=cr;
                    p1=p1+cr;
                    printf("跳过[%ld]字节",cr);
                }
                status(t_cnt,src_left,dst_left);
                break;
            case EINVAL:// 不完整的编码，可理解未输入缓存有单个字节没有被处理
                if(src_left != 1){
                    // unexpect error
                    printf("应该是1，但其实是%ld\n",src_left);
                    status(t_cnt,src_left,dst_left);
                }
                break;
            default:
                status(t_cnt,src_left,dst_left);
            }
            memcpy(b1,p1,src_left);
            src_size=SRC_SIZE-src_left;
        } else {
            if(src_left){
                printf("应该是0，但其实是%ld\n",src_left);
                fwrite(b2,1,DST_SIZE-dst_left,stdout);
                status(t_cnt,src_left,dst_left);
                retcode=-1;
                goto err_exit;
            }
            src_size=SRC_SIZE;
        }
        fwrite(b2,1,DST_SIZE-dst_left,f2);
        
    }

    retcode=0;
err_exit:
    free(b1);
    free(b2);
    return retcode;
}

int main(int argc,char **argv)
{
    FILE *f=fopen("/home/arm/txt/orig.txt","rb");
    FILE *o=fopen("/home/arm/txt/yttlj.txt","wb");
    iconv_t cd=iconv_open("utf-8","gbk");
    int errcnt=gbk2utf8(cd,f,o);
    iconv_close(cd);
    fclose(f);
    fclose(o);
    if(errcnt){
        printf("有%d字符被转换\n",errcnt);
    }
}
