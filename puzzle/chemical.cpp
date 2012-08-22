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
	cout << "usage:\n    chemical file_name\n" << endl;
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
bool is_right_one(const Path *p,const PATHES &s){
	for(int i=0;i<s.size();i++){
		if(p->dst == s[i]->src) return false;
	}
	return true;
}

void resolve(const PATHES &solu,const Node *next_one){
	int result_r=sum_right(solution);
	if(result_r>0){
		if(sum_right(solu)>result_r)return;
	}
	if(solu.size() == nodes.size()-1){
		for(int i=0;i<pathes.size();i++){
			if(pathes[i]->src == next_one && pathes[i]->dst == solu[0]->src){
				if(result_r==0){
					solution.insert(solution.begin(),solu.begin(),solu.end());
					solution.push_back(pathes[i]);
				} else {
					int temp_r=sum_right(solu)+pathes[i]->right;
					if(result_r>temp_r){
						solution.clear();
						solution.insert(solution.begin(),solu.begin(),solu.end());
						solution.push_back(pathes[i]);
					}
				}
				return;
			}
		}
	}
	for(int i=0;i<pathes.size();i++){
		if(pathes[i]->src == next_one){
			if(is_right_one(pathes[i],solu)){
				PATHES temp_solu;
				temp_solu.insert(temp_solu.begin(),solu.begin(),solu.end());
				temp_solu.push_back(pathes[i]);
				resolve(temp_solu,pathes[i]->dst);
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
	
//	PATHES solution;
	nodes.clear();
	
	for(int i=0;i<pathes.size();i++){
		NODES::const_iterator itr=find(nodes.begin(),nodes.end(),pathes[i]->src);
		if(itr==nodes.end()){
			nodes.push_back(*itr);
		}
	}
	solution.clear();
	for(int i=0;i<nodes.size();i++){
		for(int j=0;j<pathes.size();j++){
			if(pathes[j]->src == nodes[i]){
				PATHES solu;
				solu.push_back(pathes[j]);
				resolve(solu,pathes[j]->dst);
				break;
			}
		}
	}
	sort(solution.begin(),solution.end(),sort_right);
	print(solution);
	cout << "---------------------------------" << endl;
	sort(solution.begin(),solution.end(),sort_seq);
	for(int i=0;i<solution.size();i++){
		cout << solution[i]->seq << " ";
	}
	cout << endl << sum_right(solution) << endl;
	
	
	for(int i=0;i<pathes.size();i++){
		delete pathes[i];
	}
	for(int i=0;i<nodes.size();i++){
		delete nodes[i];
	}
}