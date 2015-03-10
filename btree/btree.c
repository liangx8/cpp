#include <malloc.h>
#include <stdio.h>
#include <wchar.h>

#include "btree.h"

Btree *btree_new(int (*c)(ULONG,ULONG)){
  Btree *bt=(Btree*)malloc(sizeof(Btree));
  bt->comp=c;
  return bt;
}
node *new_node(ULONG e){
  node *n;
  n=(node*)malloc(sizeof(node));
  n->e=e;
  n->l=n->r=NULL;
  n->lh=n->rh=0;
  return n;
}


node *recur_add(node *top,ULONG e,int (*cp)(ULONG,ULONG)){
  int cpv;
  if (top==NULL) {
    return new_node(e);
  }
  cpv=(*cp)(top->e,e);
  if (cpv==0) {
    top->e=e;
    return top;
  }
  if (cpv>0) {
    top->l=recur_add(top->l,e,cp);
  } else {
    top->r=recur_add(top->r,e,cp);
  }
  return top;
}
// 如果有重复返回旧的元素
ULONG btree_add(Btree *tree,ULONG el){
  tree->top=recur_add(tree->top,el,tree->comp);
  return 0;
}
void walk(node *top,int (*callback)(ULONG)){
  if (top == NULL) {
    return;
  }
  if (top->l != NULL) {
    walk(top->l,callback);
  }
  if ((*callback)(top->e)) {
    return;
  }
  if (top->r != NULL) {
    walk(top->r,callback);
  }
  return;
}

void btree_each(Btree* bt,int (*callback)(ULONG)){
  walk(bt->top,callback);
}

