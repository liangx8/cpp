#ifndef _DF13_COMMON_H
#define _DF13_COMMON_H
#include <git2.h>
void check_error(int error,const char *action);
const char* find_repo_root(const char *);

#endif

