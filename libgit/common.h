#ifndef _DF13_COMMON_H
#define _DF13_COMMON_H
#include <git2.h>
void check_error(int error,const char *action);
const char* find_repo_root(const char *);

struct bytebuffer{
  char *head;
  int size;
  int idx;
  int capcity;
};

void buf_add(struct bytebuffer *buf,int c);
void buf_clean(struct bytebuffer *buf);
void buf_init(struct bytebuffer *buf);
void buf_reset(struct bytebuffer *buf);

#endif

