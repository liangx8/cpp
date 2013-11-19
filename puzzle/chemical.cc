#include <iostream>
#include <fstream>
#include <clocale>
#include <cstdlib> // atoi()
#include <vector>
#include <algorithm>
#include <stack>

#include <unistd.h>

#include "data.h"
/*
bool greater(Path *l,Path *r){
	return l->weight>r->weight;
}
*/
struct complete_use{
	char *n;
	unsigned int j;
};
int sum(const std::vector<Path*>& ps){
	int s=0;
	for(unsigned int i=0;i<ps.size();i++){
		ps[i]->print();
		s += ps[i]->weight;
	}
	return s;
}
bool less_equal(Path *l,Path *r){
	return l->weight<=r->weight;
}
bool order(Path *l,Path *r){
	return l->seq>r->seq;
}
void usage(const char *s){
	const wchar_t *p;
	if(!(p=utf8_decode(s))){
		std::wcout << L"出错了" << std::endl;
		return;
	}
	std::wcout << p << std::endl;
}
void duplicate(std::vector<char *> &dst, const std::vector<char *>& src){
	for(unsigned int i=0;i<src.size();i++){
		dst.push_back(src[i]);
	}
}
template <class T>
bool exists(std::vector<T> &ts,T e){
	for(unsigned int i=0;i<ts.size();i++){
		if (e == ts[i]) return true;
	}
	return false;
}
// 检测结果是否完整，每个节点都能到达任意一个其他节点
void testCompletely(	const std::vector<Path*>& ps,
			const std::vector<char *>& ns){
	std::stack<struct complete_use*> ss;
	unsigned int i,j;
	struct complete_use *cu;
	std::vector<char *> *pn;
	for(i=0;i<ns.size();i++){
		char *n=ns[i];
		bool complete=false;
		pn=new std::vector<char *>;
		pn->push_back(n);
		
start:
		for(j=0;j<ps.size();j++){
			if(pn->size() == ns.size()){
				complete=true;
				break;
			}
			if (n==ps[j]->src){
				// 检测节点是否已经遍历，如果遍历过，则退上递归。
				// 检索下一条路径。
				if (!exists(*pn,ps[j]->dst)){
					// 节点没有遍历。进入递归循环
					cu=new struct complete_use;
					cu->n=n;
					cu->j=j;
					ss.push(cu);
					n=ps[j]->dst;
					pn->push_back(ps[j]->dst);
					goto start;
middle:
					cu=ss.top();
					ss.pop();
					j=cu->j;
					n=cu->n;
					delete cu;
				}
			}
			
		}
		if(!ss.empty()) goto middle;
		delete pn;
		if(!complete){
			std::wcout << utf8_decode(ns[i]) << L" is not complete" << std::endl;
			return;
		}
	}
	std::wcout << L"OK" << std::endl;

}
bool add_unique(std::vector<char *>& ns,char *s){
	for(unsigned int i=0;i<ns.size();i++){
		if(ns[i]==s) return false;
	}
	ns.push_back(s);
	return true;
}
void add_right(	const std::vector<Path*>& ps,
		const std::vector<char*>& ns,
		std::vector<Path*>& target_ps){
	std::vector<char *> tns;
	for(unsigned int i=0;i<target_ps.size();i++){
		add_unique(tns,target_ps[i]->dst);
	}
	for (unsigned int i=0;i<ps.size();i++){
		if (tns.size()==ns.size()) break;
		if(add_unique(tns,ps[i]->dst)){
			target_ps.push_back(ps[i]);
		}
	}
}
void add_left(	const std::vector<Path*>& ps,
		const std::vector<char*>& ns,
		std::vector<Path*>& target_ps){
	std::vector<char *> tns;
	for (unsigned int i=0;i<ps.size();i++){
		if (tns.size()==ns.size()) break;
		if(add_unique(tns,ps[i]->src)){
			target_ps.push_back(ps[i]);
		}
	}
}
// 这个方法不完整。思路应该是正确，只是还差补全缺少的路径。
void solution(const std::vector<Path*>& ps,const std::vector<char *> ns){
	std::vector<Path*> solu;
	std::vector<char *> check_pool;
	add_left(ps,ns,solu);
	add_right(ps,ns,solu);
	std::wcout << sum(solu) << std::endl;
	std::wcout << L"Test completely..." << std::endl;
	testCompletely(solu,ns);
}
int main(int argc,char **argv){
	int c;
	int num;
	std::vector<Path*> pathes;
	std::vector<char*> nodes;
	std::setlocale(LC_ALL,"");
	if (argc == 1) {
		usage(argv[0]);
		return 0;
	}
	while ((c = getopt(argc,argv,"G:h")) != -1){
//		std::wcout << opt << std::endl;
		switch(c){
		case 'G':
			num=std::atoi(optarg);
			gen(num);
			return 0;
		case 'h':
		default:
			usage(argv[0]);
			return 0;
		}
	}
	std::ifstream inf;
	inf.open(argv[1],std::ifstream::in);
	if(!inf.is_open()){
		std::wcerr << L"文件打开出错"<< std::endl;
		return 0;
	}
	if (parse(&inf,&pathes,&nodes)){
		std::wcerr << L"文件格式不对" << std::endl;
	}
	inf.close();

	std::sort(pathes.begin(),pathes.end(),less_equal);
	for(unsigned int i=0;i<pathes.size();i++){
		pathes[i]->print();
	}
	std::wcout << L"=====================================" << std::endl;
	solution(pathes,nodes);
	for(unsigned int i=0;i<pathes.size();i++){
		delete pathes[i];
	}
	for(unsigned int i=0;i<nodes.size();i++){
		delete nodes[i];
	}
	return 0;
}

