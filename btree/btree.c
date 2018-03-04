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

extern Callback dbg;

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
inline int depth(node *n)
{
  if(n==NULL) return 0;
  return (n->ldepth > n->rdepth ? n->ldepth:n->rdepth)+1;
}
inline int balance(node *n)
{
  return n->rdepth-n->ldepth;
}


/*
      d               b
     / \             / \
    b   e           a   d
   / \  |  ====>    |  / \
  a   c ?           ? c   f
  |   |               |   |
  ?   ?               ?   ?
 */
node* rotate_right(node *top)
{
  node* d;
  node* b;
  //node* a=top->l->l;
  node* c;
  //node* f=top->r;
  assert(top != NULL);
  assert(top->l != NULL);

  d=top;
  b=top->l;
  c=top->l->r;


  d->l=c;
  d->ldepth=depth(c);
  b->r=d;
  b->rdepth=depth(d);
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
  b->rdepth=depth(c);
  d->l=b;
  d->ldepth=depth(d);
  return d;
  
}

node* rotate_lr(node *top)
{

  assert(top->l != NULL);
  assert(top->r != NULL);
  top->l=rotate_left(top->l);
  top->ldepth=depth(top->l);
  return rotate_right(top);
}
node* rotate_rl(node *top)
{
  Btree bt;
  assert(top->l != NULL);
  assert(top->r != NULL);
  top->r=rotate_right(top->r);
  top->rdepth=depth(top->r);
  bt.top=top;
  dbg(&bt);
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
	} else {
	  top->ldepth += recur_add(top->l,e,cp);
	}
	if(top->ldepth > top->rdepth) inc_depth=1;
  } else {
	if(top->r==NULL){
	  top->r=new_node(e);
	  top->rdepth ++;
	} else {
	  top->rdepth += recur_add(top->r,e,cp);
	}
	if(top->rdepth > top->ldepth) inc_depth=1;
  }
  
  return inc_depth;
}

ELEMENT btree_add(Btree *tree,ELEMENT el)
{
  if(tree->top == NULL) {
	tree->top=new_node(el);
	return el;
  }
  recur_add(tree->top,el,tree->comp);
  return el;
}
void walk(node *top,Callback callback)
{
  if (top == NULL) {
    return;
  }
  walk(top->l,callback);
  wprintf(L"[%d,%d]",top->ldepth,top->rdepth);
  if (callback(top->e)) {
    return;
  }
  walk(top->r,callback);
  return;
}

void btree_each(Btree* bt,Callback callback)
{
  walk(bt->top,callback);
}


void node_free(node *top,Callback callback)
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
void btree_clear(Btree *bt,  Callback callback)
{
  node_free(bt->top,callback);
}

void btree_balance(Btree *bt)
{
  bt->top=rotate_rl(bt->top);
}


/*
                      3
                     / \
                    2   8
                   /   / \
                  0   4   9
                   \   \   
                    1   5  
                         \
                          7
                         /
                        6

                      3
                     / \
                    2   4
                   /     \
                  0       8
                   \     / \
                    1   5   9
                         \
                          7
                         /
                        6


         1
       0   3
          2 4
             5

 */
