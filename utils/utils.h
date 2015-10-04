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
int buf_printf(buffer *,const char *,...);
const char* buf_dup(buffer *b,const char* src);

// array

type struct _array array;

array* ary_new(int);
void ary_free();
void ary_size();
// size of unit
int ary_uom();
void ary_add(void *);
int *ary_get(int);
int *ary_set(int,void *); 


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
