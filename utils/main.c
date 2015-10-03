#include <stdio.h>
#include "utils.h"

int main(){
  buffer *b=buf_new();
  buf_add_char(b,'a');
  buf_add_cstr(b," hello\n");
  for(int i=0;i<100;i++){
	buf_add_cstr(b," hello\n");
  }
  buf_printf(b,"%*d\n",10,20);
  buf_printf(b,"中文%2$*1$d",10,20);
  const char*p=buf_to_cstr(b);
  for(int i=0;i<buf_size(b);i++){
	if(!p[i]){
	  printf("%d ",i);
	}
  }
  printf(p);
	

  buf_free(b);

}
