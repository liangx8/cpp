#include <stdio.h>
#include <assert.h>
#include <stdarg.h> // va_start, va_end
#include <string.h>
#include "mem.h"
#include "utils.h"
#define BUFFER_SIZE 128
struct _string {
  array *const_bufs;
  size_t capacity;
  size_t idx;
  char *buf;
};

string *str_new(){
  string *str = mem_malloc(sizeof(string));
  str->const_bufs=ary_new();
  str->idx=0;
  str->capacity=BUFFER_SIZE;
  str->buf=mem_malloc(BUFFER_SIZE);
  return str;
}
void str_appendc(string *str,int c){
  str->buf[str->idx]=c;
  str->idx++;
  if(str->idx >= str->capacity){
	str->capacity += BUFFER_SIZE;
	str->buf=mem_realloc(str->buf,str->capacity);
  }
  str->buf[str->idx]='\0';
}
void str_appends(string *str,const char *cs){
  str_nappends(str,strlen(cs)+1,cs);
}
void str_nappends(string *str,size_t n,const char *cs){

  size_t size=str->idx + n;
  if(size >= str->capacity){
	str->capacity= (size / BUFFER_SIZE  +1 )*BUFFER_SIZE;
	str->buf=mem_realloc(str->buf,str->capacity);
  }
  strncpy(str->buf+str->idx,cs,n);
  str->idx=size-1;

}
const char *str_cstr(string *str){
  char *cs=mem_malloc(str->idx+1);

  strncpy(cs,str->buf,str->idx+1);
  ary_add(str->const_bufs,cs);

  return cs;
}
const char *str_dup(string *str,const char *cs){
  str->idx=0;
  str_appends(str,cs);
  return str_cstr(str);
}
void str_printf(string *str,const char *format,...){
  FILE *vfp=fopen("/dev/null","w");
  va_list ap;
  va_start(ap,format);
  int len=vfprintf(vfp,format,ap);
  va_end(ap);
  fclose(vfp);
  size_t size=str->idx + len;
  if(size >= str->capacity){
	str->capacity = (size / BUFFER_SIZE + 1) * BUFFER_SIZE;
	str->buf=mem_realloc(str->buf,str->capacity);
  }
  va_start(ap,format);
  len=vsprintf(str->buf+str->idx,format,ap);
  va_end(ap);

  str->buf[size]='\0';
  str->idx = size;
}
void str_free(string *str){
  int foreach_free(void *p){
	assert(p);
	mem_free(p);
	return 0;
  }
  ary_foreach(str->const_bufs,foreach_free);
  ary_free(str->const_bufs);
  mem_free(str->buf);
  mem_free(str);
}
void str_reset(string *str){
  str->idx=0;
}

void str_split(string *str,const char *src, int split,int (*cb)(const char *cs)){
  int len=strlen(src);
  char *buf=mem_malloc(len+1);

  int idx=0;
  ary_add(str->const_bufs,buf);
  for(int i=0;i<len;i++){
	if (src[i]==split){
	  buf[i]='\0';

	  if (cb(buf+idx)) return;
	  idx=i+1;
	}else {
	  buf[i]=src[i];
	}
  }
  buf[len]='\0';
  cb(buf+idx);
}

