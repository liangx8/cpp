/*
 * $Id$
 * $Author$
 * $Committer$
 */
#ifndef UTILS_H
#define UTILS_H
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
#endif
