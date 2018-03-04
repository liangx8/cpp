#include <malloc.h>
#include <stdio.h>
//#include <stdlib.h> // abs()
#include <wchar.h>
#include <assert.h>

#include "btree.h"
/*
 * 
 * return level
 */



typedef struct _node node;
struct _node{
  ELEMENT e;
  struct _node *l,*r;
  // 根据左右的深度 左边深位负数，右边深位正数
  int ldepth,rdepth;
};
struct _btree{
  Compare comp;
  struct _node *top;
};

inline int depth_and_inc(node *n)
{
  if(n==NULL) return 0;
  return (n->ldepth > n->rdepth ? n->ldepth:n->rdepth)+1;
}
inline int balance(node *n)
{
  return n->rdepth-n->ldepth;
}
int is_balance(node*top)
{
  int b;
  if(top==NULL) return 1;
  b = balance(top);
  if(b < -1 || b > 1) return 0;
  return is_balance(top->l) && is_balance(top->r);
}
#ifndef NDEBUG
extern Callback dbg;
#endif
/*
      d               b
     / \             / \
    b   e           a   d
   / \  |  ====>    |  / \
  a   c ?           ? c   e
  |   |               |   |
  ?   ?               ?   ?
 */
node* rotate_right(node *top)
{
  node* d;
  node* b;
  //node* a=top->l->l;
  node* c;
  //node* e=top->r;
  assert(top != NULL);
  assert(top->l != NULL);

  d=top;
  b=top->l;
  c=top->l->r;


  d->l=c;
  d->ldepth=depth_and_inc(c);
  b->r=d;
  b->rdepth=depth_and_inc(d);
  return b;
}

/*
      b                 d
     / \               / \
    a   d             b   e
    |  / \    ====>  / \  |
    ? c   e         a   c ?
      |   |         |   |
      ?   ?         ?   ?
 */
node* rotate_left(node *top)
{
  //node* a;
  node* b;
  node* c;
  node* d;
  //node* e;
  assert(top != NULL);
  assert(top->r != NULL);
  //a=top->l;
  b=top;
  c=top->r->l;
  d=top->r;
  //e=top->r->r;

  b->r=c;
  b->rdepth=depth_and_inc(c);
  d->l=b;
  d->ldepth=depth_and_inc(b);
  return d;
  
}

node* rotate_lr(node *top)
{

  assert(top->l != NULL);
  //assert(top->r != NULL);
  top->l=rotate_left(top->l);
  top->ldepth=depth_and_inc(top->l);
  return rotate_right(top);
}
node* rotate_rl(node *top)
{
  //assert(top->l != NULL);
  assert(top->r != NULL);
  top->r=rotate_right(top->r);
  top->rdepth=depth_and_inc(top->r);
  return rotate_left(top);
}

Btree *btree_new(Compare c)
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
  n->ldepth=0;
  n->rdepth=0;
  return n;
}
/*
 * 加节点后同时平衡
 */
node* balance_add(node *top,ELEMENT e,Compare cp)
{
  int cpv;
  int bal;

  if(top==NULL)return new_node(e);
  cpv=cp(top->e,e);
  if(cpv>0){
	// add to left
	if(top->l==NULL){
	  top->l=new_node(e);
	  top->ldepth=1;
	  return top;
	} else {
	  top->l=balance_add(top->l,e,cp);
	  top->ldepth = depth_and_inc(top->l);
	}
  } else {
	// add to right
	if(top->r==NULL){
	  top->r=new_node(e);
	  top->rdepth=1;
	  return top;
	} else {
	  top->r=balance_add(top->r,e,cp);
	  top->rdepth=depth_and_inc(top->r);
	}
  }
  // balance current tree
  bal=balance(top);
  assert(bal >= -2 && bal <= 2);
  if(bal == -2){
	// rotate right
	int bal1=balance(top->l);
	assert(bal1 > -2 && bal1 < 2);
	if(bal1 <= 0){
	  top=rotate_right(top);
	} else {
	  top=rotate_lr(top);
	}
  }
  if(bal == 2){
	// rotate left
	int bal1=balance(top->r);
	assert(bal1 > -2 && bal1 < 2);
	if(bal1 >= 0){
	  top=rotate_left(top);
	} else {
	  top=rotate_rl(top);
	}
  }
  return top;
}
/**
 * top root of node, 
 * e element to add
 * cp compare
 * return number of increase depth
 */
int recur_add(node *top,ELEMENT e,Compare cp)
{
  int cpv;
  int inc_depth=0;
  assert(top != NULL);
  cpv=cp(top->e,e);
  if (cpv>0) {
	if(top->l==NULL){
	  top->l=new_node(e);
	  top->ldepth ++;
	  inc_depth=1;
	} else {
	  inc_depth=recur_add(top->l,e,cp);
	  top->ldepth += inc_depth;
	}
	if(top->ldepth <= top->rdepth) inc_depth=0;
  } else {
	if(top->r==NULL){
	  top->r=new_node(e);
	  top->rdepth ++;
	  inc_depth=1;
	} else {
	  inc_depth = recur_add(top->r,e,cp);
	  top->rdepth += inc_depth;
	}
	if(top->rdepth <= top->ldepth) inc_depth=0;
  }
  
  return inc_depth;
}

ELEMENT btree_add(Btree *tree,ELEMENT el)
{

  tree->top=balance_add(tree->top,el,tree->comp);
  return el;

  /*
  if(tree->top == NULL) {
	tree->top=new_node(el);
	return el;
  }
  recur_add(tree->top,el,tree->comp);
  return el;
  */
}
void walk(node *top,Callback callback,int *step)
{
  if (top == NULL) {
    return;
  }

  walk(top->l,callback,step);
  wprintf(L"[%d,%d]",top->ldepth,top->rdepth);
  (*step)++;
  if (callback(*step,top->e)) {
    return;
  }
  walk(top->r,callback,step);
  return;
}

void btree_each(Btree* bt,Callback callback)
{
  int idx=0;
  walk(bt->top,callback,&idx);
}


void node_free(node *top,Callback callback,int *step)
{
  if(top == NULL) return;
  if(top->l != NULL){
	node_free(top->l,callback,step);
	//free(top->l);
  }
  if(top->r != NULL){
	node_free(top->r,callback,step);
	//free(top->r);
  }
  (*step)++;
  if(callback != NULL){
	callback(*step,top->e);
  }
  free(top);
}
void btree_clear(Btree *bt,  Callback callback)
{
  int idx=0;
  node_free(bt->top,callback,&idx);
}

node* balance_node(node *top)
{
  int bal;
  assert(top != NULL);
  if(top->l != NULL){
	top->l = balance_node(top->l);
	top->ldepth=depth_and_inc(top->l);
  }
  if(top->r != NULL){
	top->r = balance_node(top->r);
	top->rdepth=depth_and_inc(top->r);
  }
  while(1){
	bal=balance(top);
#ifndef NDEBUG
	if(bal < -1 || bal > 1){
	  Btree bt;
	  bt.top=top;
	  dbg(0,(ELEMENT)&bt);
	}
#endif
	if(bal > 1){
	  // turn left
	  assert(top->r != NULL);
	  if(top->r->l == NULL ){
		top=rotate_left(top);
	  } else {
		top=rotate_rl(top);
	  }
	} else if (bal < -1){
	  // turn right
	  assert(top->l != NULL);
	  if(top->l->r == NULL ){
		top=rotate_right(top);
	  } else {
		top=rotate_lr(top);
	  }
	} else
	  break;
#ifndef NDEBUG
	{
	  Btree bt;
	  bt.top=top;
	  dbg(0,(ELEMENT)&bt);
	}
#endif
  }
  return top;
}
// 有缺陷,虽然功能实现了,有效率问题
void btree_balance(Btree *bt)
{
  if(bt->top == NULL) return;
  while(!is_balance(bt->top))
	//要重复多次才能完全平衡
	bt->top=balance_node(bt->top);
}
int test_is_balance(Btree *bt)
{
  return is_balance(bt->top);
}

