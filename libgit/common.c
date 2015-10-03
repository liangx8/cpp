#include <stdio.h>
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
	//	printf("%s--%s\n",repo_root,child->d_name);
	if((strlen(child->d_name)==4)
	   && (strncmp(".git",child->d_name,4)==0)){
	  closedir(parent);
	  return 0;
	}
	child=readdir(parent);
  }
  closedir(parent);
  strcat(repo_root,"/..");
  return recur_repo_path();
}
const char* find_repo_root(const char *p){
  repo_root[0]='\0';
  strcat(repo_root,p);
  if(recur_repo_path())
	return NULL;
  return repo_root;
}
