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
int buf_add_cstr(buffer *,const char *);
const char * buf_to_cstr(const buffer *);
int buf_size(const buffer *);
// 追加到buffer后面
int buf_printf(buffer *,const char *,...);
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


// linked
typedef struct _linked linked;

linked *lnk_new();
void lnk_free(linked *);
int lnk_each(linked *,int (*)(void *));
int lnk_size(linked *);
int lnk_push(linked *,void *);
int lnk_pop(linked *,void **);

#endif
