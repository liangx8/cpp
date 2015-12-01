/* 
 * $Id$
 * $Author$
 * $Committer$
 */
#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "utils.h"
#define BUFFER_BLOCKSZ  256

struct _buffer {
  char *head;
  size_t size;
  size_t capacity;
};


buffer* buf_new(){
  buffer *b=(buffer*)malloc(sizeof(buffer));
  b->head=malloc(BUFFER_BLOCKSZ);
  b->capacity=BUFFER_BLOCKSZ;
  b->size=0;
  return b;
}

void buf_reset(buffer *b){
  b->size=0;
}
void buf_free(buffer *b){
  free(b->head);
  free(b);
}
void buf_add_char(buffer *b,char c){
  b->head[b->size]=c;
  b->size++;
  if(b->size>=b->capacity){
	b->capacity += BUFFER_BLOCKSZ;
	b->head=realloc(b->head,b->capacity);
  }
}
void buf_add_n(buffer *b,const char *src,size_t n){
  size_t size=b->size + n;
  if(size >= b->capacity){
	b->capacity = (size / BUFFER_BLOCKSZ + 1)*BUFFER_BLOCKSZ;
	b->head=realloc(b->head,b->capacity);
  }


  strncpy(b->head+b->size,src,n);
  b->size=size;
}
size_t buf_add_cstr(buffer *b,const char *cstr){
  size_t len=strlen(cstr);
  buf_add_n(b,cstr,len);
  return len;
}
const char* buf_to_cstr(const buffer *b){
  b->head[b->size]='\0';
  return b->head;
}
size_t buf_size(const buffer *b){
  return b->size;
}
size_t buf_printf(buffer *b,const char *format,...){
  char *strp;
  va_list ap;
  va_start(ap,format);
  size_t len=vasprintf(&strp,format,ap);
  va_end(ap);
  buf_add_n(b,strp,len);
  free(strp);
  return len;
}
// the return value is available until b is destoryed
const char* buf_dup(buffer *b,const char* src){
  size_t idx=b->size;
  buf_add_cstr(b,src);
  buf_add_char(b,'\0');
  return b->head+idx;
}
// 把src中的内容由split一个个提取出来,由buf管理
const char* buf_split(buffer *b,const char*src,int split,size_t *start){
  size_t idx=b->size;
  assert(start);

  if(src[*start]=='\0') return NULL;
  while(1){

	if(src[*start]==split){
	  buf_add_char(b,'\0');
	  (*start)++;
	  break;
	}
	buf_add_char(b,src[*start]);
	(*start)++;
	if(src[*start]=='\0'){
	  buf_add_char(b,'\0');
	  break;
	}
  }
  return b->head+idx;
}
void *buf_malloc(buffer *b,size_t size){
  size_t idx=b->size;
  b->size += size;
  if(b->size>b->capacity){
	b->capacity=(b->size / BUFFER_BLOCKSZ + 1)*BUFFER_BLOCKSZ;
	b->head=realloc(b->head,b->capacity);
  }
  return b->head+idx;
}
