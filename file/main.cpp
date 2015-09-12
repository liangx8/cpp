// -*-coding:utf-8-*-
#include <libintl.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <string>
#include <locale>
#include <algorithm>

#define LOCALEDIR "locale/"
#define PACKAGE "filesize"


void walk(const char *top,void (*p)(const char *,LPWIN32_FIND_DATA));
int winmode(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow);
std::vector<std::pair<DWORD,std::string *>* > dirs;

bool cmp(std::pair<unsigned long,std::string *>* l,std::pair<unsigned long,std::string *>* r){
  return l->first > r->first;
}
void proc(const char *fn,LPWIN32_FIND_DATA s) {
  std::pair<DWORD,std::string *> *p;
  p=new std::pair<unsigned long,std::string*>(s->nFileSizeHigh*MAXDWORD+s->nFileSizeLow,new std::string(fn));
  //  printf("%d,%x,%s\n",__LINE__,s->dwFileAttributes,fn);
  dirs.push_back(p);
}
void usage(const char *n){
  printf("%s [-l <num>] %s\n",n,gettext("path"));
  printf("\t-l <num>\tnum, ");
  printf(gettext("display first num line"));
  printf("\n");
}

int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
  bindtextdomain(PACKAGE,LOCALEDIR);
  textdomain(PACKAGE);
  setlocale(LC_ALL,"C");
  int nCmdLen=strlen(lpCmdLine);
  if(nCmdLen==0){
	return winmode(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
  }

  int limit=-1;
  char path[100];
  bool before=1;
  path[0]=0;
  char *pp=path;

  for(int idx=0;idx<nCmdLen;idx++){
	if(lpCmdLine[idx]=='-'){
	  idx++;
	  if(idx>=nCmdLen){
		goto args_err;
	  }
	  if(lpCmdLine[idx]=='l'){
		char num[100];
		char *p=num;
		idx ++;
		if(idx>=nCmdLen){
		  goto args_err;
		}
		while(lpCmdLine[idx]==' ')idx++;
		while(1){
		  char c=lpCmdLine[idx];
		  if(idx>=nCmdLen || c == ' '){
			break;
		  }
		  *p=lpCmdLine[idx];
		  idx++;
		  p++;
		}
		*p=0;
		limit=atoi(num);
		continue;
	  }
	  goto args_err;
	}
	if(before){
	  if(lpCmdLine[idx]==' ')continue;
	  before=0;
	  *pp=lpCmdLine[idx];
	  pp++;
	  continue;
	}
	if(!before){
	  if(lpCmdLine[idx]==' '){
		break;
	  }
	  *pp=lpCmdLine[idx];
	  pp++;
	}
  }
  *pp='\0';

  if(strlen(path)==0){
	printf("%s\n",gettext("Expected a path"));
	usage("???");
	return 0;
  }

  walk(path,proc);
  std::sort(dirs.begin(),dirs.end(),cmp);
  for(size_t i=0;i<dirs.size();i++){
	DWORD size=dirs[i]->first;
	char uom=' ';
	if(size > 1024*1024){
	  size=size/1024/1024;
	  uom='M';
	} else if (size >1024){
	  size=size/1024;
	  uom='K';
	}
	printf("%u%c\t%s\n",size,uom,dirs[i]->second->c_str());
	if(limit>0){
	  limit --;
	  if(limit==0) break;
	}
  }
  for(size_t i=0;i<dirs.size();i++){
	delete dirs[i]->second;
	delete dirs[i];
  }
  return 0;
 args_err:
  usage("???");
  return 0;
}

