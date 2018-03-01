#include <malloc.h>
#include <stdio.h>
#include <wchar.h>

#include "btree.h"

Btree *btree_new(int (*c)(ELEMENT,ELEMENT))
{
  Btree *bt=(Btree*)malloc(sizeof(Btree));
  bt->comp=c;
  return bt;
}
node *new_node(ELEMENT e)
{
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
node *recur_add(node *top,ELEMENT e,int (*cp)(ELEMENT,ELEMENT), int *premote)
{
  int cpv;
  int p;
  int depth;

  if (top==NULL) {
	if(premote!=NULL)
	  *premote=1;
    return new_node(e);
  }
  // 获取当前节点的平衡深度
  depth= top->balance >= 0 ? top->balance : - top->balance;
  cpv=cp(top->e,e);
  if (cpv>0) {
	top->l=recur_add(top->l,e,cp,&p);
	top->balance -= p;
  } else {
    top->r=recur_add(top->r,e,cp,&p);
	top->balance += p;
  }
  if(premote != NULL){
	int new_depth= top->balance >= 0 ? top->balance : - top->balance;
	// 平衡深度增加，必然整个树都增加了1。
	// 平衡深度减少，节点必然是加到浅的叶子，对整个树的深度没有影响，因为这个是加节点。不会有减少可能
	*premote = new_depth > depth ? 1 : 0;
  }

  return top;
}
// 如果有重复返回旧的元素
ELEMENT btree_add(Btree *tree,ELEMENT el)
{
  tree->top=recur_add(tree->top,el,tree->comp,NULL);
  return 0;
}
void walk(node *top,int (*callback)(ELEMENT))
{
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

void btree_each(Btree* bt,int (*callback)(ELEMENT))
{
  walk(bt->top,callback);
}
void node_free(node *top,int (*callback)(ELEMENT))
{
  if(top == NULL) return;
  if(top->l != NULL){
	node_free(top->l,callback);
	//free(top->l);
  }
  if(top->r != NULL){
	node_free(top->r,callback);
	//free(top->r);
  }
  if(callback != NULL){
	callback(top->e);
  }
  free(top);
}
void btree_clear(Btree *bt,  int (*callback)(ELEMENT))
{
  node_free(bt->top,callback);
}

node *node_balance(node *top)
{

  return top;
}
void btree_balance(Btree *bt)
{
  bt->top=node_balance(bt->top);
}
