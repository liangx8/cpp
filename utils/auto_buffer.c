#include <stdio.h>
#include <assert.h>
#include "mem.h"
#include "utils.h"
#define AB_BLOCK_SIZE 16


struct _buffer_block{
  struct _buffer_block *next;
  size_t size,obj_size;
  char buffer[0];
};
struct _auto_buffer {
  struct _buffer_block *head;
};
// 变化结构体
struct _buffer_block *_new_block(size_t obj_size,size_t size,struct _buffer_block *next){
  obj_size=obj_size+sizeof(int);// 为每个目标对象添加一个标记,长度为系统对齐的长度
  struct _buffer_block *block=mem_malloc(sizeof(struct _buffer_block)+obj_size*size);
  block->next=next;
  block->size=size;
  block->obj_size=obj_size;
  for(size_t i=0;i<size;i++){
	int *f=(int *)(block->buffer+i*obj_size);
	*f=0;// 新的对象,全设为可用
  }
  return block;
}
auto_buffer *ab_new(size_t unit_size){
  auto_buffer *ab=mem_malloc(sizeof(auto_buffer));
  
  ab->head=_new_block(unit_size,AB_BLOCK_SIZE,NULL);

  return ab;
}
void *_ab_first_available(struct _buffer_block *block){
  assert(block);
  struct _buffer_block *curr=block;
  while(curr){
	for(size_t i=0;i<curr->size;i++){
	  void *obj=curr->buffer+i*curr->obj_size;
	  if ( *((int*)obj) == 0 ){
		*((int*)obj) = 1;
		return obj + sizeof(int);
	  }
	}
	curr=curr->next;
  }
  return NULL;
}
void *ab_obj_malloc(auto_buffer *ab){
  void *obj=_ab_first_available(ab->head);
  if (!obj){
	// no free, create new block
	ab->head= _new_block(ab->head->obj_size - sizeof(int),AB_BLOCK_SIZE,ab->head);
	obj=ab->head->buffer;
	*((int*)obj)=1;
	return obj+sizeof(int);// 跳开前面的标记
  }
  return obj;
}
void ab_obj_free(auto_buffer *ab,void *obj){
  int *pi=(int*)obj;
  pi--;
  *pi=0;
}

void ab_free(auto_buffer *ab){
  struct _buffer_block *next,*start=ab->head;
  while(start){
	next=start->next;
	mem_free(start);
	start=next;
  }
  mem_free(ab);
}
