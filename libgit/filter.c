#include <stdio.h>
#include <locale.h>
#include <git2.h>
#include <libintl.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "common.h"
#define LOCALEDIR "locale/"
#define PACKAGE "filter"

#ifndef UNICODE
#define UNICODE
#endif

#define CLEAN  0
#define EXPEN  1

struct options{
  int action;
  const char *dir;
};
struct commitinfo{
  const char *c_name,*c_email;
  const char *a_name,*a_email;
  const char* message;
  char strid[GIT_OID_HEXSZ+1];
  buffer *b;
};


static void get_commitinfo(git_repository *repo,struct commitinfo *info,const git_oid *oid){
  //  print_id(id);
  git_commit *commit;
  const git_signature *sig;
  check_error(git_commit_lookup(&commit,repo,oid),gettext("lookup commit"));
  sig=git_commit_author(commit);
  info->a_name=buf_dup(info->b,sig->name);
  info->a_email=buf_dup(info->b,sig->email);
  sig=git_commit_committer(commit);
  info->c_name=buf_dup(info->b,sig->name);
  info->c_email=buf_dup(info->b,sig->email);
  info->message=buf_dup(info->b,git_commit_message(commit));
  
  git_oid_tostr(info->strid,sizeof(info->strid),oid);

  //  print_commit(commit);
  git_commit_free(commit);
}
static void parse_option(struct options *opt,int argc,char **argv){
  opt->action=EXPEN;
  opt->dir=".";
  for(int i=1;i<argc;i++){
	if(argv[i][0]=='-'){
	  switch(argv[i][1]){
	  case 'c':
		opt->action=CLEAN;
		break;
	  case 'e':break;
	  default:
		printf(gettext("invalid option %s\n"),argv[i]);
		exit(-1);
	  }
	} else {
	  opt->dir=argv[i];
	}
  }
}
const git_oid* get_head_id(git_repository *repo){
  git_reference *head=NULL;
  const git_oid *oid;
  check_error(git_repository_head(&head,repo),gettext("Get Head"));
  oid=git_reference_target(head);
  git_reference_free(head);
  return oid;

}
const char * author_holder="$Author$";
const char * committer_holder="$Committer$";
int filter_expen(FILE *out,const char*src,int size,const struct commitinfo *info){

  for(int i=0;i<size;i++){
	if(src[i]=='$'){
	  if(i+sizeof(author_holder)<=size) {
		if(strncmp(src+i,author_holder,8)==0){
		  fprintf(out,"$Author: %s <%s>$",info->a_name,info->a_email);
		  i += 7;
		  continue;
		}
	  }
	  if(i+sizeof(committer_holder)<=size){
		if(strncmp(src+i,committer_holder,11)==0){
		  fprintf(out,"$Committer: %s <%s>$", info->c_name,info->c_email);
		  i += 10;
		  continue;
		}
	  }
	  fputc('$',out);
	} else {
	  fputc(src[i],out);
	}
  }

  //  fprintf(out,"$Id$",info->strid);


  return 0;
}
int filter_clean(FILE *out,const char*src,int size,const struct commitinfo *info){
  fprintf(stderr,"clean\n");
  fwrite(src,size,1,out);
  return 0;
}

int filter(FILE *in,FILE *out,int(*transfer)(FILE*,const char *,int,const struct commitinfo *),const struct commitinfo *info){

  buffer *buf=buf_new();

  while(1){
	if(transfer){
	  int c=fgetc(in);
	  if(c==EOF)break;
	  buf_add_char(buf,c);
	  if(c=='\n'){
		transfer(out,buf_to_cstr(buf),buf_size(buf),info);
		buf_reset(buf);
	  }
	}else{
	  //根据测试，这里还是按单字节看待文件流
	  int c=fgetc(in);
	  if(c==EOF)break;
	  fputc(c,out);
	}
  }
  buf_free(buf);

  return 0;
}

int main(int argc,char **argv){
  const char* repo_root;
  git_repository *repo=NULL;
  struct options opt;
  setlocale(LC_ALL,"");
  bindtextdomain(PACKAGE,LOCALEDIR);
  textdomain(PACKAGE);

  parse_option(&opt,argc,argv);
  if((repo_root=find_repo_root(opt.dir))!=NULL){
	struct commitinfo info;
	int retval;
	git_libgit2_init();
	check_error(git_repository_open(&repo,repo_root),gettext("open repository"));
	info.b=buf_new();
	get_commitinfo(repo,&info,get_head_id(repo));
	git_repository_free(repo);
	if(opt.action==CLEAN)
	  retval= filter(stdin,stdout,filter_clean,NULL);
	else
	//	return filter(stdin,stdout,NULL,&info);
	  retval= filter(stdin,stdout,filter_expen,&info);
	buf_free(info.b);
	return retval;
  }
  return -1;
}
