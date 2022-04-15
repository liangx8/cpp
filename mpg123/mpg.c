#include <stdio.h>
#include <mpg123.h>
const char *decoder_type="generic";
void run(void)
{
	int err;
	mpg123_handle* handle = mpg123_new(decoder_type,&err);
	mpg123_delete(handle);
}
int main(int argc,char **argv)
{
	const char **ava;
	ava=mpg123_decoders();
	int idx=0;
	printf("available:\n");
	while(*ava){
		printf("  %d %s\n",idx,*ava);
		idx ++;
		ava++;
	}
	ava = mpg123_supported_decoders();
        printf("supported:\n");
        while(*ava){
                printf("  %d %s\n",idx,*ava);
                idx ++;
                ava++;
        }
	run();

}
