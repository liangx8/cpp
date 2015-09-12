// -*-coding:utf-8-*-
/**
在mingw for windows 时， long的长度为 4，在linux编译时， long 的长度为8
所以，当在windows下，文件大小不能大于 2^32（4,294,967,295） ，否则会不准确
如果有文件大于4G,应该使用windows api
 */
//#include <stdint.h>
#include <cstdlib> // atoi()
//#include <sys/types.h>
#include <getopt.h>
#include <iconv.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <clocale>
#include <cwchar>

#include <utility>
#include <iostream>
#include <string>
#include <cstring> // str* funciotns
#include <vector>
#include <algorithm>
class IConv{
private:
  iconv_t handle;
  bool same;
  char *outbuf;
  size_t outlen;

  char *inp;
  size_t inlen;
public:
  IConv(const char *tocode,const char *fromcode)throw (std::wstring&){
	if(strcmp(tocode,fromcode)){
	  same=false;
	} else {
	  same=true;
	}
	outbuf=inp=NULL;
	outlen=inlen=0;
	handle=iconv_open(tocode,fromcode);
	if (handle == (iconv_t)-1){
	  if(errno== EINVAL)
		throw std::wstring(L"iconv_open:不支持编码转换");
	  throw std::wstring(L"iconv_open:其他错误");
	}

  }
  virtual ~IConv(){
	if(inlen > 0){
	  delete [] inp;
	}
	if(outlen>0){
	  delete [] outbuf;
	}
	iconv_close(handle);
  }
  //*outp 中指定返回的buf不需要被释放，IConv会自动管理内存
  size_t convert(const char *inbuf,size_t len,char **outp) throw (std::wstring&){
	char *p1,*p2;
	if(outlen==0){
	  outlen=len*4;
	  if(outlen>16*1024*1024){
		throw std::wstring(L"内容转换结果太多");
	  }
	  outbuf=new char[outlen];
	}
	if(outlen<len*4){
	  delete [] outbuf;
	  outlen=len*4;
	  outbuf = new char[outlen];
	}

	if(same){
	  //	  wprintf(L"%d>same,%d\n",__LINE__,len);
	  *outp=outbuf;
	  memcpy(outbuf,inbuf,len);
	  return len;
	}
	if(inlen==0){
	  inlen=len;
	  inp=new char[len];
	}
	if(inlen<len){
	  delete [] inp;
	  inp=new char[len];
	  inlen=len;
	}
	memcpy(inp,inbuf,len);
	size_t size=outlen;
	p1=inp;
	p2=outbuf;

	size_t t=iconv(handle,&p1,&len,&p2,&size);
	if(t==(size_t)-1){
	  switch(errno){
	  case EILSEQ:
		throw std::wstring(L"input buffer is left pointing to the beginning of the invalid multibyte sequence");
	  case EINVAL:
		throw std::wstring(L"input buffer is left pointing to the beginning of the incomplete multibyte sequence");
	  case E2BIG:
		throw std::wstring(L"output buffer has no more room for the next converted character");
	  }
	}
	*outp=outbuf;
	return outlen-size;
  }
};
IConv *cvt;

void usage(const char *n){
  wprintf(L"%s [-l <num>][-c encoding] path\n",n);
  wprintf(L"\t-l <num>\tnum, display first num line\n");
  wprintf(L"\t-c <coding>\tcoding, file system encoding \"GBK\", \"UTF-8\" \n");
}
bool cmp(std::pair<unsigned long,std::wstring *>* l,std::pair<unsigned long,std::wstring *>* r){
  return l->first > r->first;
}
void walk(const char *top,void (*p)(const char *,struct stat *)) throw (std::wstring&){
  struct stat sb;
  int err_code;
  DIR *dir;
  if((err_code=stat(top,&sb))==-1){
	throw std::wstring(L"访问文件出错了");
  }
  switch (sb.st_mode & S_IFMT){
  case S_IFBLK:  wprintf(L"block device\n");            break;
  case S_IFCHR:  wprintf(L"character device\n");        break;
  case S_IFDIR:
	//wprintf("directory\n");
	dir=opendir(top);
	if(dir==NULL){
	  wprintf(L"error(%s):",top);
	  switch(errno){
	  case EACCES:wprintf(L"Permission denied!");break;
	  default:wprintf(L"其他错误！");
	  }
	} else {
	  struct dirent *sd=NULL;
	  char fullname[1024];
	  int toplen=strlen(top);
	  while(1){
		sd=readdir(dir);
		if(sd==NULL) break;
		if(strlen(sd->d_name)==1 && sd->d_name[0] == '.') continue;
		if(strlen(sd->d_name)==2 && sd->d_name[0]== '.' && sd->d_name[1] == '.') continue;
		strcpy(fullname,top);
		fullname[toplen]='/';
		fullname[toplen+1]=0;
		strcat(fullname,sd->d_name);
		walk(fullname,p);
	  }
	  closedir(dir);
	}
	break;
  case S_IFIFO:  wprintf(L"FIFO/pipe\n");               break;
  case S_IFREG:
	//printf("regular file\n");
	//	wprintf(L"%d>%s\n",__LINE__,top);
	p(top,&sb);
	break;
  default:       wprintf(L"unknown?\n");                break;

  }
}
std::vector<std::pair<unsigned long,std::wstring *>* > dirs;
void proc(const char *fn,struct stat *s) {
  //  char *wfn;
  const wchar_t *wc;
  int len=strlen(fn)+1;
  std::pair<unsigned long,std::wstring *> *p;
  cvt->convert(fn,len,(char **)&wc);

  //  std::pair<int64_t,std::wstring>* p=
  p=new std::pair<unsigned long,std::wstring*>(s->st_size,new std::wstring(wc));

  //  wprintf(L"%d>1*%d,",__LINE__,len,wc);



  // while(*wc){
  // 	//  	wprintf(L"%lc",*wc);
  // 	str.push_back(*wc);
  // 	wc++;
  // }
  // wprintf(L"\n");
  // wprintf(L"%d>2*%s\n",__LINE__,fn);

  dirs.push_back(p);
}
int main(int argc, char **argv){
  setlocale(LC_ALL,"");
  if(argc==1){
	usage(argv[0]);
	return 0;
  }
  int opt;
  int limit=-1;
  const char *encoding="GBK";
  while((opt=getopt(argc,argv,"c:l:"))!=-1){
	switch(opt){
	case 'l':
	  limit = atoi(optarg);
	  break;
	case 'c':
	  encoding=optarg;
	  break;
	default:
	  usage(argv[0]);
	  break;
	}
  }
  if(optind>=argc){
	wprintf(L"Expected path\n");
	usage(argv[0]);
	return 0;
  }
  try{
	const char *wc;
	if(sizeof(wchar_t)==4){
	  wc="UCS-4LE";
	}else{
	  wc="UCS-2LE";
	}
	IConv h(wc,encoding);
	cvt=&h;
	walk(argv[optind],proc);
	std::sort(dirs.begin(),dirs.end(),cmp);
	for(size_t i=0;i<dirs.size();i++){
	  unsigned long size=dirs[i]->first;
	  wchar_t uom=L' ';
	  if(size>1024*1024){
		size=size/1024/1024;
		uom=L'M';
	  }else if (size>1024){
		size=size/1024;
		uom=L'K';
	  }
	  wprintf(L"%u%lc\t%ls\n",size,uom,dirs[i]->second->c_str());
	  //	  std::wcout << *(dirs[i]->second) << std::endl;

	  if(limit>0){
		limit --;
		if (limit==0) break;
	  }
	}

  }catch(std::wstring &e){
	std::wcout << e << std::endl;
  }
  for(size_t i=0;i<dirs.size();i++){
	delete dirs[i]->second;
	delete dirs[i];
  }
  //wprintf(L"中文");
  // IConvUtil c("GBK");

  return 0;
}

