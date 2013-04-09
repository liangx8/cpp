/*
 * 穷举遍历方式求解，相当低效
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

class Node{
private:
	const string *name;
public:
	Node(const string&v){
		name=new string(v);
	}
	virtual ~Node(){
		//cout <<"destruct:" <<*name << endl;
		delete name;
	}
	bool operator ==(const string &r) const {
		return *name==r;
	}
	const string &get_name() const {
		return *name;
	}
};
class Path{
public:
	int seq;
	Node *src;
	Node *dst;
	int	right;
	void print(void) const {
		cout << 'M'<<seq << '\t' << src->get_name() << '\t' << dst->get_name() << '\t' << right << endl;
	}
	virtual ~Path(){
		//cout << src->get_name() << "=>" << dst->get_name() << endl;
		
	}
};
typedef vector<Path*> PATHES;
typedef vector<Node*> NODES;


void usage(){
	cout << "usage:\n    \033[0;33mchemical file_name\033[0m\n" << endl;
}

bool sort_right(Path *lhs,Path *rhs){
	return lhs->right < rhs->right;
}
bool sort_seq(Path *lhs,Path *rhs){
	return lhs->seq < rhs->seq;
}

void print(const PATHES& p){
	for(PATHES::const_iterator i=p.begin();i != p.end();i++){
		(*i)->print();
	}
}

void generate_puzzle(int number){
	ofstream out("puzzle.txt");
	int c=0;
	srand(time(NULL));
	for(int i=0;i<number;i++){
		for(int j=0;j<number;j++){
			if(i==j)continue;
			out << 'M' << ++c << '\t' << 'C' << i+1 << '\t' << 'C' << j+1 << '\t' << rand() << endl;
		}
	}
	out.close();
}

PATHES pathes,solution;
NODES nodes;
int sum_right(const PATHES &p){
	int sum=0;
	for(int i=0;i<p.size();i++){
		sum += p[i]->right;
	}
	return sum;
}

bool nextif(Path *p,const PATHES &ps){
	for(int i=0;i<ps.size();i++){
		if(p->dst == ps[i]->src){
			return false;
		}
	}
	//p->print();
	return true;
}
void copy(PATHES &dst,const PATHES &src){
	dst.clear();
	for(int i=0;i<src.size();i++){
		dst.push_back(src[i]);
	}
}
Path* find_final(const PATHES &solu){
	Node *src=solu[solu.size()-1]->dst;
	Node *dst=solu[0]->src;
	for(int i=0;i<pathes.size();i++){
		if(pathes[i]->src == src && pathes[i]->dst == dst){
			return pathes[i];
		}
	}
	return NULL;
}
void resolve(Node *nd,const PATHES &solu){
	if(solu.size() == nodes.size()-1){
		Path *final=find_final(solu);
		int right=sum_right(solution);
		if (right==0){
			copy(solution,solu);
			solution.push_back(final);
			return;
		}
		if(right>sum_right(solu)+final->right){
			copy(solution,solu);
			solution.push_back(final);
		}
		return;
	}
	for(int i=0;i<pathes.size();i++){
		if(pathes[i]->src == nd){
			if (nextif(pathes[i],solu)){
				PATHES temp_solu;
				copy(temp_solu,solu);
				temp_solu.push_back(pathes[i]);
				resolve(pathes[i]->dst,temp_solu);
			}
		}
	}
}
// parse input file
void parse(ifstream &infile){
	string holder;
//	NODES nodes;
//	PATHES pathes;
	while(infile >> holder){
		Path *p=new Path;
		string temp=string(holder.erase(holder.begin()),holder.end());
		p->seq=atoi(temp.c_str());
		infile >> holder;
		NODES::const_iterator itr;
		Node *node_temp;
		for(itr=nodes.begin();itr != nodes.end();itr++){
			if(*(*itr) == holder){
				node_temp=*itr;
				break;
			}
		}
		if(itr == nodes.end()){
			node_temp=new Node(holder);
			nodes.push_back(node_temp);
		}
		p->src=node_temp;

		infile >> holder;
		for(itr=nodes.begin();itr != nodes.end();itr++){
			
			if(*(*itr) == holder){
				node_temp=*itr;
				break;
			}
		}
		if(itr == nodes.end()){
			node_temp=new Node(holder);
			nodes.push_back(node_temp);
		}
		p->dst=node_temp;
		infile >> holder;
		p->right=atoi(holder.c_str());
		pathes.push_back(p);

	}
}
int main(int argc, char *argv[]){
	if(argc == 1){
		usage();
		return 0;
	}
	if(strcmp(argv[1],"-G")==0){
		int c=atoi(argv[2]);
		generate_puzzle(c);
		return 0;
	}
	const char *filename;
	if(strcmp(argv[1],"-s")==0){
		filename="puzzle.txt";
	} else {
		filename=argv[1];
	}
	
	ifstream infile(filename);
	if(!infile){
		cerr << "File is not found!" << endl;
		return -1;
	}
	parse(infile);
	infile.close();
	sort(pathes.begin(),pathes.end(),sort_right);
	if(strcmp(argv[1],"-s")==0){
		print(pathes);
		return 0;
	}
	for(int i=0;i<pathes.size();i++){
		NODES::const_iterator itr=find(nodes.begin(),nodes.end(),pathes[i]->src);
		if(itr==nodes.end()){
			nodes.push_back(*itr);
		}
	}
	PATHES solu;
//void resolve(const PATHES &solu,const Node *next_one){	
	resolve(pathes[0]->src,solu);
	print(solution);
	cout << sum_right(solution);
	for(int i=0;i<pathes.size();i++){
		delete pathes[i];
	}
}