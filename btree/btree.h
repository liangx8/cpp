#ifndef __BTREE_H__
#define __BTREE_H__
#include <wchar.h>

typedef void* ELEMENT;

typedef struct _btree Btree;
typedef int (*Callback)(ELEMENT);
typedef int (*Compare)(ELEMENT,ELEMENT);

Btree *btree_new(Compare);
ELEMENT  btree_add(Btree*,ELEMENT);
void btree_each(Btree*,Callback);
void btree_clear(Btree*,Callback);
void btree_balance(Btree*);


void btree_print(Btree*,int,wchar_t* (*)(ELEMENT));
#endif
