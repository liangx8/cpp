#ifndef PARSER_CMD_H
#define PARSER_CMD_H
#include <getopt.h>
#include <vector>
//extern char *optarg;
//extern int optind, opterr,optopt;
class ParserCmd{
private:
	int opt_index;
	const char *opts;
	const struct option *long_opts;
	int ac;
	char **av;
	std::vector<char*> args;
public:
	ParserCmd(const char *options,const struct option long_options[]);
	~ParserCmd();
	void parser(int argc,char **argv);
	int next(void);
	char *opt_arg(void)const;
	const char *opt_name(void)const ;
	char* arg(int index) const;
	int count_arg()const;
};
#endif
