#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <stdlib.h>




#include "btree.h"
#define MAX 40
wchar_t buf[10];

int cp(ELEMENT l,ELEMENT r){
  return (int)(l-r);
}
wchar_t *str(ELEMENT e){
  swprintf(buf,10,L"%02d",(long)e);
  return buf;
}
int fn(ELEMENT e){
  wprintf(L"%d,",e);
  return 0;
}

Callback dbg;

int incepte(ELEMENT e){
  btree_each((Btree*)e,fn);
  wprintf(L"\n");
  btree_print((Btree*)e,2,str);
  return 0;
}

int main(int argc,char **argv){
  setlocale(LC_ALL,"");
  Btree *bt=btree_new(cp);
  int i;
  int cnt=MAX;
  int *matrix;
  dbg=incepte;

  if(argc > 1){
	cnt = atoi(argv[1]);
	if(cnt==0)cnt=MAX;
  }
  if(argc > 2){
	int seed=atoi(argv[2]);
	if(seed != 0)srand(seed);
  }
  matrix=(int *)malloc(sizeof(int)*cnt);
  for(i=0;i<cnt;i++){
	matrix[i]=i;
  }
  for(i=0;i<cnt;i++){
	int tmp=matrix[i];
	int idx=rand() % cnt;
	matrix[i]=matrix[idx];
	matrix[idx]=tmp;
  }
  for(i=0;i<cnt;i++) {
	wprintf(L"%d,",matrix[i]);
    btree_add(bt,(ELEMENT)(long)(matrix[i]));
  }
  wprintf(L"\n");
  free(matrix);
  btree_each(bt,fn);
  wprintf(L"\n");
  btree_print(bt,2,str);

  btree_balance(bt);

  btree_each(bt,fn);
  wprintf(L"\n");
  btree_print(bt,2,str);


  btree_clear(bt,fn);
  free(bt);
  wprintf(L"\n%d,%d\n",sizeof(void*),sizeof(int));
  wprintf(L"%d,%d\n",sizeof(void*),sizeof(ELEMENT));
  return 0;
}
