#include <malloc.h>
#include <stdio.h>
#include <wchar.h>

#include "btree.h"

Btree *btree_new(int (*c)(ELEMENT,ELEMENT)){
  Btree *bt=(Btree*)malloc(sizeof(Btree));
  bt->comp=c;
  return bt;
}
node *new_node(ELEMENT e){
  node *n;
  n=(node*)malloc(sizeof(node));
  n->e=e;
  n->l=n->r=NULL;
  n->balance=0;
  return n;
}

/**
 * top old root of node, 
 * e element to add
 * cp compare
 * premote, change of depth of new tree
 */
node *recur_add(node *top,ELEMENT e,int (*cp)(ELEMENT,ELEMENT), int *premote){
  int cpv;
  int p;

  if (top==NULL) {
	if(premote!=NULL)
	  *premote=1;
    return new_node(e);
  }
  cpv=cp(top->e,e);
  if (cpv>0) {
	top->l=recur_add(top->l,e,cp,&p);
	top->balance -= p;
  } else {
    top->r=recur_add(top->r,e,cp,&p);
	top->balance += p;
  }
  if(premote != NULL)
	*premote= top->balance ==0 ? 0:1;

  return top;
}
// 如果有重复返回旧的元素
ELEMENT btree_add(Btree *tree,ELEMENT el){
  tree->top=recur_add(tree->top,el,tree->comp,NULL);
  return 0;
}
void walk(node *top,int (*callback)(ELEMENT)){
  if (top == NULL) {
    return;
  }
  if (top->l != NULL) {
    walk(top->l,callback);
  }
  wprintf(L"(%d)",top->balance);
  if (callback(top->e)) {
    return;
  }
  if (top->r != NULL) {
    walk(top->r,callback);
  }
  return;
}

void btree_each(Btree* bt,int (*callback)(ELEMENT)){
  walk(bt->top,callback);
}

