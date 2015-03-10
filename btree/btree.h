#ifndef __BTREE_H__
#define __BTREE_H__
#include <wchar.h>

typedef unsigned long ULONG;
typedef struct _node {
  ULONG e;
  struct _node *l,*r;
  int lh,rh;
} node;
typedef struct {
  int (*comp)(ULONG,ULONG);
  node *top;
} Btree;
Btree *btree_new(int (*)(ULONG,ULONG));
ULONG  btree_add(Btree*,ULONG);
void  btree_each(Btree*,int (*)(ULONG));
void btree_print(node *top,int,wchar_t* (*)(ULONG));
#endif
