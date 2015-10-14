/*
 * $Id$
 * $Author$
 * $Committer$
 */
#ifndef UTILS_H
#define UTILS_H

// smart buff

typedef struct _buffer buffer;
buffer* buf_new();
void buf_reset(buffer *);
void buf_free(buffer *);
void buf_add_char(buffer *,char );
size_t buf_add_cstr(buffer *,const char *);
const char * buf_to_cstr(const buffer *);
size_t buf_size(const buffer *);
// 追加到buffer后面
size_t buf_printf(buffer *,const char *,...);
const char* buf_dup(buffer *b,const char* src);

// array

typedef struct _array array;
typedef int (*compare)(void*,void*);
typedef void (*extrusion_cb)(void *);
array* ary_new();
void ary_free(array*);
size_t ary_size(array*);

void ary_add(array*,void *);
int ary_get(array*,size_t,void **);
int ary_set(array*,size_t,void *);
int ary_sort(array*,compare);
// size 重新大小
// callback 多余出来的对象被callback调用,一般用于释放
void ary_resize(array*,size_t size,extrusion_cb callback);
// return -1 not found
int ary_find(array *ary,void *key,void **out,compare cmp);
// src 要分割的串
// split 分割字符
// start 开始以及结果偏移
const char* buf_split(buffer *b,const char*src,int split,size_t *start);
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

#endif
