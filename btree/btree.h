#ifndef __BTREE_H__
#define __BTREE_H__
#include <wchar.h>

typedef void* ELEMENT;
typedef struct _node {
  ELEMENT e;
  struct _node *l,*r;
  // 根据左右的深度 左边深位负数，右边深位正数
  int balance;
} node;
typedef struct {
  int (*comp)(ELEMENT,ELEMENT);
  node *top;
} Btree;
Btree *btree_new(int (*)(ELEMENT,ELEMENT));
ELEMENT  btree_add(Btree*,ELEMENT);
void btree_each(Btree*,int (*)(ELEMENT));
void btree_clear(Btree *,int (*)(ELEMENT));
void btree_balance(Btree *);

void btree_print(node *top,int,wchar_t* (*)(ELEMENT));
#endif
