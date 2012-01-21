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
};
typedef vector<Path*> PATHES;
typedef vector<Node*> NODES;


void usage(){
	cout << "usage:\n    chemical file_name\n" << endl;
}
class eqstring{
private:
	const string cmp;
public:
	eqstring(const string &v):cmp(v){}
	bool operator()(const string *s) const {
		return cmp == *s;
	}
};
int sort_right(Path *lhs,Path *rhs){
	return lhs->right < rhs->right;
}
int sort_seq(Path *lhs,Path *rhs){
	return lhs->seq < rhs->seq;
}
bool exists(Path* p,const PATHES& pathes){
	for(PATHES::const_iterator itr=pathes.begin();itr != pathes.end();itr++){
		if( *itr == p) return true;
	}
	return false;
}
int sum_right(const PATHES::const_iterator start,const PATHES::const_iterator end){
	int sum=0;
	for(PATHES::const_iterator itr=start;itr != end;itr++){
		sum += (*itr)->right;
	}
	return sum;
}
bool eq_seq(Path *lhs,Path *rhs){
	return lhs->seq == rhs->seq;
}
void print(const PATHES& p){
	for(PATHES::const_iterator i=p.begin();i != p.end();i++){
		(*i)->print();
	}
}

bool insert_if(PATHES &solu,Path *p){
	for(int i=0;i<solu.size();i++){
		if(p == solu[i]) return false;
		if(p->dst == solu[i]->dst) return false;
		if(p->src == solu[i]->src) return false;
		
	}
	solu.push_back(p);
	return true;
}
// return false - not closed
// return true  - closed
bool check_close(const PATHES &p){
	int size=p.size();
	for(int i=0;i<size;i++){
		Node *start=p[i]->src;
		Node *next=p[i]->dst;
		for(int j=0;j<size;j++){
			if(p[i]==p[j]) continue;
			if(next == p[j]->src){
				next = p[j]->dst;
				if(next == start) return true;
				j=0;
			}
		}
	}
	return false;
}
bool resolv(int index,const PATHES &pathes,const NODES& nodes,PATHES& solution){
	int size=nodes.size();
	solution.push_back(pathes[index]);
	int debug_count=0;
	while(1){
//		if(debug_count++>1000) {
//			cout << "------------------------" << endl;
//			print(solution);
//			return false;
//		}
		if(solution.size()==size) break;
		if(check_close(solution)){
			// 结果 不完整
//			cout << "imcompleted results!" << endl;
//			print(solution);
			return false;
		}
		for(int i=0;i<pathes.size();i++){
			if(insert_if(solution,pathes[i])) {
				break;
			}
		}
	}
/*
	sort(solution.begin(),solution.end(),sort_seq);
	cout << sum_right(solution.begin(),solution.end()) << endl;
	for(int i=0;i<solution.size();i++){
		cout << solution[i]->seq << ' ';
	}
	cout << endl;
*/
	return true;
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
int main(int argc, char *argv[]){
	if(argc == 1){
		usage();
		return 0;
	}
	if(strcmp(argv[1],"gen")==0){
		int c=atoi(argv[2]);
		generate_puzzle(c);
		return 0;
	}
	ifstream infile(argv[1]);
	if(!infile){
		cerr << "File is not found!" << endl;
		return -1;
	}

	// parse input file
	string holder;
	NODES nodes;
	PATHES pathes;
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
	sort(pathes.begin(),pathes.end(),sort_right);
//	print(pathes);
	PATHES best_solu;
	for(int i=0;i<pathes.size();i++){
		PATHES solution;
		if(!resolv(i,pathes,nodes,solution))continue;
		if(best_solu.size()==0){
			best_solu.insert(best_solu.begin(),solution.begin(),solution.end());
		} else {
			if(sum_right(best_solu.begin(),best_solu.end())>sum_right(solution.begin(),solution.end())){
				best_solu.clear();
				best_solu.insert(best_solu.begin(),solution.begin(),solution.end());
			}
		}
	}
	sort(best_solu.begin(),best_solu.end(),sort_seq);
	cout << sum_right(best_solu.begin(),best_solu.end()) << endl;
	for(int i=0;i<best_solu.size();i++){
		cout << best_solu[i]->seq << ' ';
	}
	cout << endl;
	
	// release memory
	for(PATHES::const_iterator itr=pathes.begin();itr != pathes.end();itr++){
		delete *itr;
	}
	for(NODES::const_iterator itr=nodes.begin();itr != nodes.end();itr++){
		delete *itr;
	}
}