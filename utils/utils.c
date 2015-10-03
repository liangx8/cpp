#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "utils.h"
#define BUFFER_BLOCKSZ  256

struct _buffer {
  char *head;
  int size;
  int capacity;
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
void buf_add_n(buffer *b,const char *src,int n){
  int size=b->size + n;
  if(size >= b->capacity){
	b->capacity = (size / BUFFER_BLOCKSZ + 1)*BUFFER_BLOCKSZ;
	b->head=realloc(b->head,b->capacity);
  }


  strncpy(b->head+b->size,src,n);
  b->size=size;
}
void buf_add_cstr(buffer *b,const char *cstr){
  int len=strlen(cstr);
  buf_add_n(b,cstr,len);
}
const char* buf_to_cstr(const buffer *b){
  b->head[b->size]='\0';
  return b->head;
}
int buf_size(const buffer *b){
  return b->size;
}
int buf_printf(buffer *b,const char *format,...){
  char *strp;
  va_list ap;
  va_start(ap,format);
  int len=vasprintf(&strp,format,ap);
  va_end(ap);
  buf_add_n(b,strp,len);
  free(strp);
  return len;
}
