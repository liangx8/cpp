#include <malloc.h>
#include <assert.h>
//#include <stdio.h>
#include "utils.h"
#include "mem.h"

struct _array {
  void **ar;
  size_t size,capacity;
  int sorted;
  compare cmp;
};

//typedef int (*compare)(void*,void*);
int sorted_find(array *ary,void *key,void **out,size_t *idx,size_t start,size_t end);
array* ary_new(){
  array *ary=(array*)mem_malloc(sizeof(array));
  ary->ar=(void**)mem_malloc(sizeof(void **)*128);
  ary->size=0;
  ary->capacity=128;
  ary->sorted=0;
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
void sorted_add(array *ary,void *obj){
  assert(ary->sorted && ary->cmp);
  size_t pos;
  if(ary->size>0)
	sorted_find(ary,obj,NULL,&pos,0,ary->size);
  else
	pos=0;
  ary->size++;

  for(size_t i=ary->size;i>pos;i--){
	ary->ar[i]=ary->ar[i-1];
  }
  ary->ar[pos]=obj;
  if(ary->size>=ary->capacity){
	ary->capacity += 128;
	ary->ar=(void **)mem_realloc(ary->ar,sizeof(void **)*ary->capacity);
  }
}
void ary_add(array* ary,void *obj){
  if(ary->sorted){
	return sorted_add(ary,obj);
  }
  ary->ar[ary->size]=obj;
  ary->size++;
  if(ary->size>=ary->capacity){
	ary->capacity += 128;
	ary->ar=(void **)mem_realloc(ary->ar,sizeof(void **)*ary->capacity);
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
  if(ary->sorted){
	// it's not available while array is sorted
	return -1;
  }
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
  ary->cmp=cmp;
  bubble_sort(ary,cmp);
  ary->sorted=1;
  return 0;
}
int sorted_find(array *ary,void *key,void **out,size_t *idx,size_t start,size_t end){
  assert(ary->sorted && ary->cmp);
  assert(start<end);
  if(start+1 == end){
	int result=ary->cmp(key,ary->ar[start]);
	if(result){
	  if (idx){
		//#error "需要判断是在start的前面还是后面"
		if(result<0)
		  *idx=start;
		else
		  *idx=start+1;
	  }
	  return -1;
	} else {
	  if(idx)
		*idx=start;
	  if(out){
		*out=ary->ar[start];
	  }
	  return 0;
	}
  }
  int mid=(start+end)/2;
  assert(mid!=start);
  int result=ary->cmp(key,ary->ar[mid]);
  if(result <0){

	return sorted_find(ary,key,out,idx,start,mid);
  }
  if(result >0){
	return sorted_find(ary,key,out,idx,mid,end);
  }
  if(out)
	*out=ary->ar[mid];
  if(idx)
	*idx=mid;
  return 0;
}
// return -1 object not found
int ary_find(array *ary,void *key,void **out,size_t *idx){
  if(ary->sorted){
	size_t size=ary->size;
	if(size){
	  return sorted_find(ary,key,out,idx,0,size);
	}
	if(idx){
	  *idx=0;
	}
	return -1;
  }
  perror("current array is not sorted!");
  return -1;
}

//typedef void (*extrusion_cb)(void *);
// size 重新设置大小
// callback 多余出来的对象被callback调用,一般用于释放
void ary_resize(array* ary,size_t size,int (*callback)(void *)){
  if(size > ary->size){
	ary->size=size;
	if(ary->size >= ary->capacity){
	  ary->capacity= (size / 128 + 1) * 128;
	  ary->ar = (void **)mem_realloc(ary->ar,sizeof(void**)*ary->capacity);
	}
	return;
  }
  for(size_t s=size; s<ary->size;s++){

	callback(ary->ar[s]);// ignore return
  }
  ary->size=size;
}

int ary_foreach(array *ary, int (*each_cb)(void *)){
  for(size_t i=0;i<ary->size;i++){
	int r;
	r=each_cb(ary->ar[i]);
	if(r) return r;
  }
  return 0;
}
