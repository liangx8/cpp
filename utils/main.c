#include <stdio.h>
#include <stdlib.h> // srand(),rand()
#include <time.h> // time()
#include <assert.h>
#include "utils.h"

struct integer {
  int value;
};



void test_array(){
  auto_buffer *ints=ab_new(sizeof(struct integer));
  int one(void *obj){
	printf("%d ",((struct integer*)obj)->value);
	return 0;
  }
  int cmp(void *l,void *r){
	return *((int *)r)-*((int *)l);
  }
  array *ary=ary_new();
  srand(time(NULL));
  for(int i=0;i<20;i++){
	struct integer *pi=ab_obj_malloc(ints);
	pi->value=rand()%1000;
	ary_add(ary,pi);
  }

  printf("===========================================arry test\n");
  ary_foreach(ary,one);
  ary_sort(ary,cmp);
  printf("\n");
  ary_foreach(ary,one);

  void *a;
  size_t idx=0;
  ary_get(ary,0,&a);
  if(ary_find(ary,a,NULL,&idx)){
	printf("\nexpected found a itme\n");
  }
  if(idx != 0)
	printf("\nexpected position 5 but %ld\n",idx);

  struct integer *added=ab_obj_malloc(ints);
  added->value=rand()%1000;
  if(!ary_find(ary,(void *)added,NULL,&idx)){
	printf("\nexpected not found");
  }else {
	printf("\n%d is expected at position:%ld\n",added->value,idx);
  }
  ary_add(ary,(void *)added);
  ary_foreach(ary,one);

  printf("\n");
  ary_resize(ary,0,one);
  for(int i=0;i<20;i++){
	struct integer *added=ab_obj_malloc(ints);
	added->value=rand()%1000;
	ary_add(ary,added);
  }
  printf("\n=========================================\n");
  ary_foreach(ary,one);

  printf("\nsize of struct integer %ld\n",sizeof(struct integer));
  ab_free(ints);
  ary_free(ary);
}
void test_writers(){
  writers *ws=wrt_new();
  wrt_add_handler(ws,stdout);
  wrt_add_name(ws,"main.log");
  if(wrt_open(ws)){
	printf("open writers error");
  }
  wrt_printf(ws,"hello %s\n","world");
  wrt_close(ws);
}
struct test_ab{
  int x,y;
  char ca[8];

};

void test_auto_buffer(){
  size_t size=sizeof(struct test_ab);
  size_t align_size=sizeof(int);
  printf("struct size:%ld,align size:%ld\n",size,align_size);
  auto_buffer *ab=ab_new(size);
  struct test_ab *a=ab_obj_malloc(ab);
  struct test_ab *b=ab_obj_malloc(ab);
  printf("%lx,%lx,%ld\n",(long)a,(long)b,(long)b-(long)a);
  ab_obj_free(ab,b);
  b=ab_obj_malloc(ab);
  printf("%lx,%lx,%ld\n",(long)a,(long)b,(long)b-(long)a);
  assert((long)b-(long)a == size + align_size);
  int *p=(int*)b;
  p--;
  printf("status:%d\n",*p);
  ab_obj_free(ab,b);
  printf("status:%d\n",*p);
  ab_free(ab);
}
void test_string(){
  string *str=str_new();
  int i=1;
  int cb(const char *cs){
	printf("[%d,%s]\n",i++,cs);
	return 0;
  }
  printf("=============================\n");
  str_appendc(str,'1');
  str_appendc(str,'\n');
  str_appends(str,"字符串缓冲\n");
  printf("a:%s",str_cstr(str));
  str_reset(str);
  str_printf(str,"number: %d\n",12);
  const char *s=str_cstr(str);
  printf("b:%s",s);
  printf("c:\n");
  str_split(str,"/12344/222/中文/abcd/ee/",'/',cb);
  str_reset(str);
  str_appends(str,"12");
  str_appends(str,"345");
  str_appendc(str,'\n');
  printf("d:%s",str_cstr(str));
  str_reset(str);
  for(int i=0;i<300;i++){
	str_printf(str,"sentance %03d\n",i);
  }
  printf("e:\n");

  s=str_cstr(str);
  str_free(str);
}
int main(){


  test_writers();
  test_string();
  test_auto_buffer();
  test_array();
}
