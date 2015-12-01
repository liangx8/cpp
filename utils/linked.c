#include <malloc.h>
#include <assert.h>
#include "utils.h"

struct _lnk_node{
  struct _lnk_node *next;
  void *obj;
};
struct _linked{
  struct _lnk_node *head;
  int size;
};


linked *lnk_new(){
  linked *lnk=(linked *)malloc(sizeof(linked));
  lnk->head=NULL;
  lnk->size=0;
  return lnk;
}
void lnk_free(linked *lnk){
  struct _lnk_node *p;
  assert(lnk);
  p=lnk->head;
  while(p){
	struct _lnk_node *n=p->next;
	free(p);
	p=n;
  }
  free(lnk);
}
int lnk_each(linked *lnk,int (*each)(void *)){
  assert(lnk && each);
  struct _lnk_node *p=lnk->head;
  while(p){
	int rt;
	if((rt=each(p->obj))!=0){
	  return rt;
	}
	p=p->next;
  }
  return 0;
}
int lnk_pop(linked *lnk,void **objp){
  assert(lnk);
  if(lnk->head){
	struct _lnk_node *p=lnk->head;
	if(objp){
	  *objp=p->obj;
	}
	lnk->head=p->next;
	free(p);
	lnk->size--;
	return 0;
  }
  return -1;
}
int lnk_push(linked *lnk,void *obj){
  struct _lnk_node *p=lnk->head;
  struct _lnk_node *n=(struct _lnk_node *)malloc(sizeof(struct _lnk_node));
  n->obj=obj;
  lnk->head=n;
  n->next=p;
  lnk->size++;
  return 0;
}
int lnk_size(linked *lnk){
  assert(lnk);
  return lnk->size;
}
