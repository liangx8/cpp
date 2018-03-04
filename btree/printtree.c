#include <stdio.h>
#include <malloc.h>
#include <wchar.h>

#include "btree.h"


struct _node{
  ELEMENT e;
  struct _node *l,*r;
  // 根据左右的深度 左边深位负数，右边深位正数
  int _d1;// place holder for this module don't use it
  int _d2;
};
struct _btree{
  int (*comp)(ELEMENT,ELEMENT);
  struct _node *top;
};
wchar_t* (*cov)(ELEMENT);
struct array {
  int len;
  int *val;
};
int width;
int exists(int v, struct array *a){
  int i;
  for (i=0;i<a->len;i++){
    if (v==a->val[i]) return 1;
  }
  return 0;
}
void pt(struct _node *top,int *col,struct array *ncol){
  struct array nncol;
  int i;
  wchar_t *val;
  if (top == NULL) {
    wprintf(L"$\n");
    return;
  }

  nncol.len=ncol->len+1;
  nncol.val=(int*)malloc(sizeof(int)*nncol.len);
  for (i=0;i<ncol->len;i++) {
    nncol.val[i]=ncol->val[i];
  }
  val = (*cov)(top->e);
  (*col) ++;
  wprintf(val);
  if (top->l == NULL && top->r == NULL) {
    wprintf(L"=\n");
    (*col) --;
    free(nncol.val);
    return;
  }
  wprintf(L"-+-");
  pt(top->l,col,ncol);
  for (i=0;i<*col-1;i++) {
    int j;
    for(j=0;j<width;j++){
      wprintf(L" ");
    }
    if(exists(i,ncol)){
      wprintf(L"   ");
    } else {
      wprintf(L" | ");
    }
  }
  for(i=0;i<width;i++){
    wprintf(L" ");
  }
  wprintf(L" +-");
  nncol.val[nncol.len-1]=*col-1;
  pt(top->r,col,&nncol);
  (*col)--;
  free(nncol.val);
}


// top
// w 转换对象成为char* 后的长度
void btree_print(Btree *bt,int w,wchar_t *(*str)(ELEMENT)){
  int col=0;
  struct array ary={0,NULL};
  width=w;
  cov=str;
  pt(bt->top,&col,&ary);
}
