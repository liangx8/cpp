/*
 * $Id$
 * $Author: wang gang(linux box) <wang.linux.gang@gmail.com>$
 * $Committer: wang gang(linux box) <wang.linux.gang@gmail.com>$
 */
#ifndef UTILS_H
#define UTILS_H
// byte buffer
typedef struct _string string;
string *str_new();
void str_appendc(string *,int);
void str_appends(string *,const char*);
void str_nappends(string *,size_t,const char*);
void str_printf(string *str,const char* format, ...);
const char* str_cstr(string *);
void str_free(string *);
void str_reset(string *);
void str_split(string *,const char*,int,int (*)(const char*));
const char *str_dup(string *str,const char *cs);
// array

typedef struct _array array;
typedef int (*compare)(void*,void*);
//typedef void (*extrusion_cb)(void *);
array* ary_new();
void ary_free(array*);
size_t ary_size(array*);

void ary_add(array*,void *);
int ary_get(array*,size_t,void **);
int ary_set(array*,size_t,void *);
int ary_sort(array*,compare);
// size 重新大小
// callback 多余出来的对象被callback调用,一般用于释放
void ary_resize(array*,size_t size,int (*callback)(void *));
// return -1 not found
int ary_find(array *ary,void *key,void **out,size_t *idx);

// 迭代
// each_cb 返回非0,中断迭代
int ary_foreach(array *ary, int (*each_cb)(void *));

// linked
typedef struct _linked linked;

linked *lnk_new();
void lnk_free(linked *);
int lnk_each(linked *,int (*)(void *));
int lnk_size(linked *);
int lnk_push(linked *,void *);
int lnk_pop(linked *,void **);



// balance tree
#define BTREE_DEL_NOT_FOUND -2
typedef struct _btree btree;

btree* btr_new(int (*cmp)(void*,void*));
void btr_free(btree *btr);
int btr_add(btree *btr,void *);
int btr_replace(btree *btr,void *n,void **old);
int btr_del(btree *btr,void *,void **);

// writers 同时对多个
typedef struct _writers writers;
writers *wrt_new();
void wrt_add_handler(writers *wrt,FILE *h);
int wrt_printf(writers *wrt,const char *format, ...);
int wrt_open(writers *wrt);
int wrt_close(writers *wrt);
void wrt_add_name(writers *wrt,const char *name);

// auto_buffer
typedef struct _auto_buffer auto_buffer;
auto_buffer *ab_new(size_t unit_size);
void ab_free(auto_buffer *ab);
void *ab_obj_malloc(auto_buffer *ab);
void ab_obj_free(auto_buffer *ab, void *p);
#endif
