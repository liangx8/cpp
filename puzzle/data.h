#ifndef __DATA_H_
#define __DATA_H_
#include <vector>
#include <fstream>
class Path {
public:
	int seq;
	char *src;
	char *dst;
	int weight;
	void print(void);
};
int parse(std::ifstream *ifs,std::vector<Path*> *ps,std::vector<char *> *ns);
const wchar_t* utf8_decode(const char* sz);
void gen(int dim);



#endif
