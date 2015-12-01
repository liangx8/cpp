#include <malloc.h>


void *mem_malloc(size_t size){
  void *p=malloc(size);
  if(p) {
	//	printf("malloc :%lx\n",(long)p);
	return p;
  }
  perror("malloc memory error");
  return NULL;
}
void *mem_realloc(void *ptr, size_t size){
  void *p=realloc(ptr,size);
  if(p) {
	//	printf("realloc: old %lx, new %lx\n",(long)ptr,(long)p);
	return p;
  }
  perror("realloc memory error");
  return ptr;
}
void mem_free(void *ptr){
  //  printf("free :%lx\n",(long)ptr);
  free(ptr);
}
