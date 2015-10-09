#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <libgen.h> //dirname()
#include <libintl.h>
#include <string.h> // strerror
//#include <git2.h>
#include <dirent.h>
#include <errno.h> //errno
#include "common.h"
void check_error(int error,const char *action){
  if(!error){
	return;
  }

  const git_error *e = giterr_last();
  if(e)
	fprintf(stderr,"%s %d/%d: %s\n",action,error,e->klass,e->message);
  else
	fprintf(stderr,gettext("Unknown error!!!\n"));
  exit(error);
}

static char repo_root[512];
static int recur_repo_path(){
  DIR *parent=opendir(repo_root);
  if (!parent){
	fprintf(stderr,gettext("open dir '%s' error:%s\n"),repo_root,strerror(errno));
	return -1;
  }
  struct dirent *child=readdir(parent);
  while(child){
	//printf("%s--%s\n",repo_root,child->d_name);
	if((strlen(child->d_name)==4)
	   && (strncmp(".git",child->d_name,4)==0)){
	  closedir(parent);
	  return 0;
	}
	child=readdir(parent);
  }
  closedir(parent);
  strcpy(repo_root,dirname(repo_root));
  if(repo_root[0]=='/' && repo_root[1]=='\0'){
	return -1;
  }
  return recur_repo_path();
}
const char* find_repo_root(const char *p){
  char *pp;
  int freepp;
  if(p){
	pp=realpath(p,NULL);
	if(pp){
	  freepp=1;
	  strncpy(repo_root,pp,512);
	} else {
	  fprintf(stderr,gettext("get realpath error:%s\n"),strerror(errno));
	  return NULL;
	}
  } else {
	pp=getcwd(repo_root,512);
	assert(pp==repo_root);
  }
  if(recur_repo_path()){
	fprintf(stderr,"'%s'%s\n",pp,gettext(" is not a git repository (or any of the parent directories)"));
	if(freepp)free(pp);
	return NULL;
  }
  if(freepp)free(pp);
  return repo_root;
}
