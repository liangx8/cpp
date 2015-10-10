#include <stdio.h>
#include <git2.h>
#include "common.h"
#include "utils.h"

void ref_details(git_reference *ref){

  char strid[10];
  const git_oid *id= git_reference_target(ref);
  git_oid_tostr(strid,10,id);
  printf("\ttarget:%s\n",strid);
}
int ref_callback(git_reference *ref,void *payload){
  printf("ref:%s\n",git_reference_name(ref));
  ref_details(ref);
  return 0;
}
struct object{
  git_otype otype;
  char strid[GIT_OID_HEXSZ+1];
  size_t size;
};
struct pl{
  int count;
  git_odb *odb;
  array *ary;
};
int each_object(const git_oid *id, void *payload){
  struct pl *p=payload;
  git_odb_object *out;


  check_error(git_odb_read(&out,p->odb,id),"odb_read");
  struct object *oo=(struct object *)malloc(sizeof(struct object));
  oo->size=git_odb_object_size(out);
  git_oid_tostr(oo->strid,GIT_OID_HEXSZ+1,id);
  /*
  if(osize>1024*1024) {
	osize /= 1024 *1024;
	uom='M';
  } else if (osize > 10*1024){
	osize /= 1024;
	uom='K';
	}*/
  oo->otype=git_odb_object_type(out);
  git_odb_object_free(out);
  ary_add(p->ary,(void*)oo);
  //printf("%05d: %s %5ld%c %s\n",p->count++,strid,osize,uom,git_object_type2string(otype));
  return 0;
}
int cmp(void *l,void *r){
  struct object *lv,*rv;
  lv=(struct object*)l;
  rv=(struct object*)r;
  return rv->size-lv->size;
}
void obj_free(void *o){
  free(o);
}
void print_obj(struct object *obj,size_t seq){
  printf("%05ld: %s %5ld %s\n",seq,obj->strid,obj->size,git_object_type2string(obj->otype));
}
void odb_details(git_repository *repo){
  struct pl payload;
  payload.count=0;
  payload.ary=ary_new();
  check_error(git_repository_odb(&payload.odb,repo),"repository odb");

  git_odb_foreach(payload.odb,each_object,&payload);
  git_odb_free(payload.odb);

  ary_sort(payload.ary,cmp);
  ary_resize(payload.ary,20,obj_free);
  for(size_t i=0;i<ary_size(payload.ary);i++){
	void *out;
	ary_get(payload.ary,i,&out);
	print_obj((struct object*)out,i);
	free(out);
  }

  ary_free(payload.ary);
}
void learning_libgit(git_repository *repo){
  git_reference_foreach(repo,ref_callback,NULL);
  odb_details(repo);
}
