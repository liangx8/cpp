#include <iostream>
#include <cstdlib>	// srand(),rand()
#include <ctime>	// time()
#include <iconv.h>
#include <string.h>
#include <errno.h>

#include "data.h"
void print_node(std::vector<char *> *ss){
	std::wcout << ss->size() << L':';
	for(unsigned int i=0;i<ss->size();i++){
		std::wcout << utf8_decode((*ss)[i]) << L' ';
	}
	std::wcout << std::endl;
}
void Path::print(void){
	std::wcout << L'M' << this->seq << L'\t' ;
	std::wcout << utf8_decode(this->src) << L'\t' ;
	std::wcout << utf8_decode(this->dst) << L'\t' ;
	std::wcout << this->weight << std::endl;
}
static wchar_t outbuf[256]; // private variable
const wchar_t* utf8_decode(const char* sz){
	size_t n_src=strlen(sz),n_dst=256*sizeof(wchar_t);
	char *src,*dst;
	src=(char*)sz;
	dst=(char*)outbuf;
	
	iconv_t cd=iconv_open("UCS-4LE","UTF-8");
	if (cd == iconv_t(-1)) {
		std::wcerr << L"不支持的编码(nerver reach here)" << std::endl;
	}
	size_t status=iconv(cd,&src,&n_src,&dst,&n_dst);
	if (status == (size_t)-1){
		if (errno == E2BIG) {
			std::wcerr << L"目标没有足够的空间" << std::endl;
		}
		if (errno == EILSEQ) {
			std::wcerr << L"不是正确的UTF8字符" << std::endl;
		}
		if (errno == EINVAL) {
			std::wcerr << L"UTF8字符不完整" << std::endl;
		}
		iconv_close(cd);
		return 0;
	}
	iconv_close(cd);
	outbuf[(256*sizeof(wchar_t)-n_dst)/sizeof(wchar_t)]=0;
	
	return outbuf;
}

void gen(int dim){
	std::srand(time(NULL));
	int seq=0;
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			if (i==j) continue;
			std::wcout << L'M' << ++seq << L'\t' << L'C' << i+1 << L'\t' << L'C'
				<< j+1 << L'\t' << std::rand()%100000 << std::endl;
		}
	}
}
/*
 不存在就增加一个，返回字串池中的值。
*/
char *intern_add(std::vector<char*> *ss,char *s){
	int len=strlen(s);
	char *new_s;
	for (unsigned int i=0;i<ss->size();i++){
		if (strcmp( (*ss)[i],s)==0){
			return (*ss)[i];
		}
	}
	new_s = new char[len+1];
	for(int i=0;i<len+1;i++){
		new_s[i]=s[i];
	}
	ss->push_back(new_s);
	return new_s;
}
int parse(std::ifstream *ifs,std::vector<Path*> *ps,std::vector<char *> *ns){
	char *buf;
	int retval=0;
	buf=new char[256];
	while (true){
		int seq,weight;
		char *src,*dst;
		if (ifs->eof()) break;
		buf[0]=0;
		*ifs >> buf;
		if(buf[0]==0) break;
		seq = std::atoi(buf+1);
		if (seq == 0) {
			retval=-1;
			std::wcerr << L"check 1" << std::endl;
			goto wrong_format;
		}
		if (ifs->eof()) {
			retval=-1;
			std::wcerr << seq << L'\t'<<utf8_decode(buf)<< std::endl;
			std::wcerr << L"check 2" << std::endl;
			goto wrong_format;
		}
		*ifs >> buf;
		src = intern_add(ns,buf);
		if (ifs->eof()) {
			retval=-1;
			std::wcerr << L"check 3" << std::endl;
			goto wrong_format;
		}
		*ifs >> buf;
		dst = intern_add(ns,buf);
		if (ifs->eof()) {
			retval=-1;
			std::wcerr << L"check 4" << std::endl;
			goto wrong_format;
		}
		*ifs >> weight;
		Path *p=new Path;
		p->seq=seq;
		p->src=src;
		p->dst=dst;
		p->weight=weight;
		ps->push_back(p);
	}
wrong_format:
	delete [] buf;
	return retval;
}
