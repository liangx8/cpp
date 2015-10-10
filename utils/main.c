#include <stdio.h>
#include <stdlib.h> // srand(),rand()
#include <time.h> // time()
#include <assert.h>
#include "utils.h"
void test_buf(){
  buffer *b=buf_new();
  buf_add_char(b,'a');
  buf_add_cstr(b," hello\n");
  for(int i=0;i<2;i++){
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
int one(void *obj){
  printf("%ld ",(long)obj);
  return 0;
}
void test_linked(){
  linked *lnk=lnk_new();
  assert(lnk_pop(lnk,NULL));
  lnk_push(lnk,(void *)1);
  lnk_push(lnk,(void *)2);
  lnk_push(lnk,(void *)3);
  lnk_push(lnk,(void *)4);
  lnk_push(lnk,(void *)5);
  lnk_push(lnk,(void *)6);
  lnk_push(lnk,(void *)7);
  lnk_push(lnk,(void *)8);
  lnk_push(lnk,(void *)9);
  printf("\n====================\n");
  lnk_each(lnk,one);

  assert(!lnk_pop(lnk,NULL));
  printf("\n====================\n");
  lnk_each(lnk,one);
  printf("\n");
  
  lnk_free(lnk);
}

int cmp(void *l,void *r){
  return (long)r-(long)l;
}
void cb(void *obj){
  printf("%ld ",(long)obj);
}
void test_array(){
  printf("arry test\n");
  array *ary=ary_new();
  srand(time(NULL));
  for(int i=0;i<20;i++){
	ary_add(ary,(void*)(long)(rand()%1000));
	
  }
  for(size_t i=0;i<ary_size(ary);i++){
	void *out;
	ary_get(ary,i,&out);
	printf("%ld ",(long)out);
  }
  ary_sort(ary,cmp);
  printf("\n");
  for(size_t i=0;i<ary_size(ary);i++){
	void *out;
	ary_get(ary,i,&out);
	printf("%ld ",(long)out);
  }
//typedef void (*extrusion_cb)(void *);
  printf("\n");
  ary_resize(ary,10,cb);
  printf("\n");
  ary_free(ary);
}
int main(){
  test_buf();
  test_linked();
  test_array();
}
