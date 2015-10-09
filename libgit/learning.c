#include <stdio.h>
#include <git2.h>
#include "common.h"

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
struct pl{
  int count;
  git_odb *odb;
};
int each_object(const git_oid *id, void *payload){
  struct pl *p=payload;
  char strid[GIT_OID_HEXSZ+1];
  git_odb_object *out;
  size_t osize;
  git_otype otype;
  int uom='B';

  git_oid_tostr(strid,GIT_OID_HEXSZ+1,id);
  check_error(git_odb_read(&out,p->odb,id),"odb_read");
  osize=git_odb_object_size(out);
  if(osize>1024*1024) {
	osize /= 1024 *1024;
	uom='M';
  } else if (osize > 10*1024){
	osize /= 1024;
	uom='K';
  }
  otype=git_odb_object_type(out);
  git_odb_object_free(out);
  printf("%05d: %s %5ld%c %s\n",p->count++,strid,osize,uom,git_object_type2string(otype));
  return 0;
}
void odb_details(git_repository *repo){
  struct pl payload;
  payload.count=0;
  check_error(git_repository_odb(&payload.odb,repo),"repository odb");

  git_odb_foreach(payload.odb,each_object,&payload);
  git_odb_free(payload.odb);
}
void learning_libgit(git_repository *repo){
  git_reference_foreach(repo,ref_callback,NULL);
  odb_details(repo);
}
