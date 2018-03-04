#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <stdlib.h> // rand()
#include <string.h>



#include "btree.h"
#define MAX 40
wchar_t buf[10];

int cp(ELEMENT l,ELEMENT r){
  return (int)(l-r);
}
wchar_t *str(ELEMENT e){
  swprintf(buf,10,L"%03d",(long)e);
  return buf;
}
int fn(int idx,ELEMENT e){
  wprintf(L"%03d,",e);
  if(idx % 10 == 0)wprintf(L"\n");
  return 0;
}
#ifndef NDEBUG
Callback dbg;
#endif
int print(int idx,ELEMENT e){
  btree_each((Btree*)e,fn);
  wprintf(L"\n");
  btree_print((Btree*)e,3,str);
  return 0;
}
#define RANGE_MAX 1000
void gen_rand_file(int seed)
{
  FILE *fp=fopen("data.txt","w");
  int *matrix=malloc(sizeof(int)*RANGE_MAX);
  for(int i=0;i<RANGE_MAX;i++){
	matrix[i]=i;
  }
  srand(seed);
  for(int i=0;i<RANGE_MAX;i++){
	int tmp=matrix[i];
	int idx=rand() % RANGE_MAX;
	matrix[i]=matrix[idx];
	matrix[idx]=tmp;
  }
  for(int i=0;i<RANGE_MAX;i++) {
	if(i>0)fprintf(fp," ");
	fprintf(fp,"%d",matrix[i]);
  }
  fclose(fp);
}
int* get_matrix(int size)
{
  if(size > RANGE_MAX)size=RANGE_MAX;
  int *m=malloc(sizeof(int)*size);
  FILE *fp=fopen("data.txt","r");
  for(int i=0;i<size;i++){
	fscanf(fp,"%d",&(m[i]));
  }
  return m;
}
int main(int argc,char **argv){
  setlocale(LC_ALL,"");
  Btree *bt=btree_new(cp);
  int i;
  int cnt=MAX;
  int *matrix;
#ifndef NDEBUG
  dbg=print;
#endif

  

  if(argc > 1){
	if(strncmp("-g",argv[1],2)==0){
	  int seed=0;
	  if(argc > 2){
		seed=atoi(argv[2]);
	  }
	  gen_rand_file(seed);
	  exit(0);
	}
  }
  if(argc > 1){
	cnt = atoi(argv[1]);
	if(cnt == 0 )cnt=MAX;
  }
  
  matrix=get_matrix(cnt);
  for(i=0;i<cnt;i++) {
	wprintf(L"%d,",matrix[i]);
    btree_add(bt,(ELEMENT)(long)(matrix[i]));
  }
  wprintf(L"\n");
  free(matrix);

  print(0,(ELEMENT)bt);
  btree_balance(bt);
  print(0,(ELEMENT)bt);

  btree_clear(bt,fn);
  wprintf(L"\n");
  free(bt);
  //wprintf(L"\n%d,%d\n",sizeof(void*),sizeof(int));
  //wprintf(L"%d,%d\n",sizeof(void*),sizeof(ELEMENT));
  return 0;
}
