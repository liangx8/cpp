#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <stdlib.h>

#include "btree.h"
wchar_t buf[10];

int cp(ELEMENT l,ELEMENT r){
  return (int)(l-r);
}
wchar_t *str(ELEMENT e){
  swprintf(buf,10,L"%03d",(long)e);
  return buf;
}
int fn(ELEMENT e){
  wprintf(L"%d,",e);
  return 0;
}
int main(int argc,char **argv){
  setlocale(LC_ALL,"");
  Btree *bt=btree_new(&cp);
  int i;
  for(i=0;i<40;i++) {
	unsigned long e=rand() % 1000;
	wprintf(L"%lu,",e);
    btree_add(bt,(ELEMENT)e);
  }
  wprintf(L"\n");
  btree_each(bt,&fn);
  wprintf(L"\n");
  btree_print(bt->top,3,&str);
  free(bt);
  wprintf(L"%d,%d\n",sizeof(void*),sizeof(int));
  wprintf(L"%d,%d\n",sizeof(void*),sizeof(ELEMENT));
  return 0;
}
