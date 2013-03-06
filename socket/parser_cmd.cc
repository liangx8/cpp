#include "parser_cmd.h"
//extern char *optarg;
ParserCmd::ParserCmd(const char *options,
			const struct option long_options[]):opts(options),long_opts(long_options){
}
ParserCmd::~ParserCmd(){
}
void ParserCmd::parser(int argc,char **argv){
	ac=argc;
	av=argv;
	opt_index=0;
	args.clear();
}
int ParserCmd::next(void){
	int i=0;
	while(optind+i<ac){
		if(av[optind+i][0] != '-'){
			args.push_back(av[optind+i]);
		}else break;
		i++;
	}
	return getopt_long(ac,av,opts,long_opts,&opt_index);
}
char *ParserCmd::opt_arg(void)const {
	return optarg;
}
const char *ParserCmd::opt_name(void)const {
	return long_opts[opt_index].name;
}
char* ParserCmd::arg(int index) const{
	return args[index];
}
int ParserCmd::count_arg(void) const{
	return args.size();
}
