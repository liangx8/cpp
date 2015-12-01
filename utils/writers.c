#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdarg.h> // va_list va_end
#include "utils.h"



struct sio{
  FILE *h;
  const char *name;
  int need_close;
};

struct _writers{
  array *ios;
  char *buf;
  size_t buf_size;
};

writers *wrt_new(){
  writers *io=(writers *)malloc(sizeof(writers));
  io->ios=ary_new();
  io->buf=NULL;
  io->buf_size=0;
  return io;
}
void wrt_add_handler(writers *io,FILE *h){
  if(h==NULL)return;
  struct sio *p=(struct sio*)malloc(sizeof(struct sio));
  p->h=h;
  p->name=NULL;
  p->need_close=0;
  ary_add(io->ios,p);
}
void wrt_add_name(writers *io,const char *name){
 struct sio *p=(struct sio*)malloc(sizeof(struct sio));
  p->name=name;
  p->h=NULL;
  p->need_close=1;
  ary_add(io->ios,p);
}
int wrt_open(writers *io){
  assert(io);
  int each_cb(void *p){
	assert(p);
	struct sio *io1=(struct sio*)p;
	if(!io1->h){
	  io1->h=fopen(io1->name,"w");
	  if(io1->h==NULL){
		return -1;
	  }
	}
	return 0;
  }
  return ary_foreach(io->ios,each_cb);
}
int wrt_close(writers *wrt){
  int each_cb(void *p){
	assert(p);
	struct sio *io1=(struct sio*)p;
	if(io1->need_close){
	  fclose(io1->h);
	}
	free(io1);
	return 0;
  }
  ary_foreach(wrt->ios,each_cb);
  if(wrt->buf){
	free(wrt->buf);
  }
  ary_free(wrt->ios);
  free(wrt);
  return 0;
}
int wrt_printf(writers *wrt,const char *format,...){
  va_list ap;

  int each_cb(void *p){
	assert(p);
	struct sio *io1=(struct sio*)p;
	if(io1->h){
	  fprintf(io1->h,wrt->buf);
	}

	return 0;
  }


  FILE *dummy=fopen("/dev/null","w");
  va_start(ap,format);
  size_t len=vfprintf(dummy,format,ap);
  va_end(ap);
  if(wrt->buf){
	if(len>wrt->buf_size){
	  wrt->buf=realloc(wrt->buf,len+1);
	  wrt->buf_size=len+1;
	}
  } else {
	wrt->buf=malloc(len+1);
	wrt->buf_size=len+1;
  }
  va_start(ap,format);
  vsprintf(wrt->buf,format,ap);
  va_end(ap);
  fclose(dummy);

  ary_foreach(wrt->ios,each_cb);
  return 0;
}
