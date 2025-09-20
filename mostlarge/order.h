#pragma once

void *create_list(int,int);
int sort_list(void *,int size);
int set_at(void *,int ,void *);
void *get_at(void *,int);

#define free_list(x) free(x)


void* str_pool_new(int);
int str_pool_put(void *,const char *);
const char* str_pool_pop(void*);
#define free_link(x) free(x)
struct stat;
typedef int(* action)(const char *,const struct stat *,void *);
