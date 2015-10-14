#include <malloc.h>
#include <assert.h>
#include "utils.h"

struct _array {
  void **ar;
  size_t size,capacity;
};

//typedef int (*compare)(void*,void*);
//typedef void (*extrusion_cb)(void *);
array* ary_new(){
  array *ary=(array*)malloc(sizeof(array));
  ary->ar=(void**)malloc(sizeof(void **)*128);
  ary->size=0;
  ary->capacity=128;
  return ary;
}
void ary_free(array* ary){
  free(ary->ar);
  free(ary);
}
size_t ary_size(array* ary){
  if(ary)
	return ary->size;
  return 0;
}

void ary_add(array* ary,void *obj){
  ary->ar[ary->size]=obj;
  ary->size++;
  if(ary->size>=ary->capacity){
	ary->capacity += 128;
	ary->ar=(void **)realloc(ary->ar,sizeof(void **)*ary->capacity);
  }
}
int ary_get(array* ary,size_t idx,void **out){
  if(out){
	if(idx>=ary->size){
	  return -1;
	}
	*out=ary->ar[idx];
  }
  return 0;
}
int ary_set(array* ary,size_t idx,void *obj){

  if(idx>=ary->size){
	return -1;
  }
  ary->ar[idx]=obj;

  return 0;

}
void bubble_sort(array *ary,compare cmp){
  assert(ary && cmp);
  for(size_t i=0;i<ary->size;i++){
	int order=1;
	for(size_t j=0;j<ary->size-i-1;j++){
	  if(cmp(ary->ar[j],ary->ar[j+1])>0){
		void *tmp=ary->ar[j];
		ary->ar[j]=ary->ar[j+1];
		ary->ar[j+1]=tmp;
		order = 0;
	  }
	}
	if (order) break;
  }
}
int ary_sort(array* ary,compare cmp){
  bubble_sort(ary,cmp);
  return 0;
}
int ary_find(array *ary,void *key,void **out,compare cmp){
  for(size_t i=0;i<ary->size;i++){
	if(cmp(ary->ar[i],key)==0){
	  if(out)
		*out=ary->ar[i];
	  return i;
	}
  }
  return -1;
}
// size 重新设置大小
// callback 多余出来的对象被callback调用,一般用于释放
void ary_resize(array* ary,size_t size,extrusion_cb callback){
  if(size > ary->size){
	ary->size=size;
	if(ary->size >= ary->capacity){
	  ary->capacity= (size / 128 + 1) * 128;
	  ary->ar = (void **)realloc(ary->ar,sizeof(void**)*ary->capacity);
	}
	return;
  }
  ary->size=size;
  for(size_t s=size; s<ary->size;s++){
	callback(ary->ar[s]);
  }
}

int ary_foreach(array *ary, int (*each_cb)(void *)){
  for(size_t i=0;i<ary->size;i++){
	int r;
	r=each_cb(ary->ar[i]);
	if(r) return r;
  }
  return 0;
}
