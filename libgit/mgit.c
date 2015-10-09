/*
 * $Id$
 * $Author$
 * $Committer$
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <locale.h>
#include <git2.h>
#include <libintl.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>
#include "utils.h"
#include "common.h"
#define LOCALEDIR "locale/"
#define PACKAGE "filter"

#ifndef UNICODE
#define UNICODE
#endif

#define FILTER_MASK 0x02
#define FILTER_MODE_MASK 0x01
#define CLEAN  0x01
#define EXPEN  0x00

struct options{
  int action;
  const char *dir;
};

struct commitinfo{
  const char *c_name,*c_email;
  const char *a_name,*a_email;
  const char *message;
  const char *head_ref_name;
  char strid[GIT_OID_HEXSZ+1];
  buffer *b;
};

static void get_commitinfo(git_repository *repo,struct commitinfo *info){

  git_commit *commit;
  const git_signature *sig;
  git_reference *head=NULL;
  const git_oid *oid;
  check_error(git_repository_head(&head,repo),gettext("Get Head"));
  oid=git_reference_target(head);

  check_error(git_commit_lookup(&commit,repo,oid),gettext("lookup commit"));
  sig=git_commit_author(commit);
  info->a_name=buf_dup(info->b,sig->name);
  info->a_email=buf_dup(info->b,sig->email);
  sig=git_commit_committer(commit);
  info->c_name=buf_dup(info->b,sig->name);
  info->c_email=buf_dup(info->b,sig->email);
  info->message=buf_dup(info->b,git_commit_message(commit));
  info->head_ref_name=buf_dup(info->b,git_reference_name(head));
  git_oid_tostr(info->strid,sizeof(info->strid),oid);
  git_reference_free(head);
  //  print_commit(commit);
  git_commit_free(commit);
}
static void filter_usage(){
  printf("%s\n",gettext("Filter mode: expend/clean the place holder from stdin to stdout"));
  printf("filter [-c|-e]\n");
  printf("\t -c, %s\n",gettext("clean place holder"));
  printf("\t -e, %s\n",gettext("expend place holder"));
}
static int parse_option(struct options *opt,int argc,char **argv){
  int optc;
  assert(opt);
  opt->action=0;

  if (strcmp(basename(argv[0]),"filter")==0){
	opt->action |= FILTER_MASK;
  }

  if(opt->action & FILTER_MASK){
	while(1){
	  optc=getopt(argc,argv,"ce");
	  if(optc==-1) break;
	  switch(optc){
	  case 'c':
		opt->action |= FILTER_MODE_MASK;
		opt->action |= CLEAN;
		break;
	  case 'e':break;
	  default:
		filter_usage();
		return -1;
	  }
	}
	opt->dir=find_repo_root(NULL);// find repository from current directory
	if(opt->dir==NULL) {
	  return -1;
	}
	return 0;
  }
  while(1){
	optc=getopt(argc,argv,"p:");
	if(optc==-1)break;
	switch(optc){
	case 'p':
	  opt->dir=find_repo_root(optarg);
	  if(opt->dir==NULL) {
		return -1;
	  }
	}
  }
  if(!opt->dir){
	  opt->dir=find_repo_root(NULL);
	  if(opt->dir==NULL) {
		return -1;
	  }
  }
  return 0;
}
const char author_holder[]="$""Author$";
const char committer_holder[]="$""Committer$";
int filter_expen(FILE *out,const char*src,int size,const struct commitinfo *info){

  for(int i=0;i<size;i++){
	if(src[i]=='$'){
	  if(i+sizeof(author_holder)<=size) {
		if(strncmp(src+i,author_holder,sizeof(author_holder)-1)==0){
		  fprintf(out,"$""Author: %s <%s>$",info->a_name,info->a_email);
		  i += sizeof(author_holder)-2;
		  continue;
		}
	  }
	  if(i+sizeof(committer_holder)<=size){
		if(strncmp(src+i,committer_holder,sizeof(committer_holder)-1)==0){
		  fprintf(out,"$""Committer: %s <%s>$", info->c_name,info->c_email);
		  i += sizeof(committer_holder)-2;
		  continue;
		}
	  }
	  fputc('$',out);
	} else {
	  fputc(src[i],out);
	}
  }


  return 0;
}
int filter_clean(FILE *out,const char*src,int size,const struct commitinfo *info){
  const char *holders[]={author_holder,committer_holder};
  const size_t sizes[]={sizeof(author_holder),sizeof(committer_holder)};
  for(size_t i=0;i<size;i++){
	if(src[i]=='$'){
	  for(int x=0;x<2;x++){

		if(strncmp(src+i,holders[x],sizes[x]-2)==0){
		  int found=0;
		  for(int j=1+i;j<size;j++){
			if(src[j]=='$') {
			  found=1;
			  i=j+1;
			  break;
			}
		  }
		  if(found){
			fprintf(out,holders[x]);
			break;
		  } 
		}
	  }
	}
	fputc(src[i],out);
  }

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
void learning_libgit(git_repository *);
int main(int argc,char **argv){
  //const char* repo_root;
  git_repository *repo=NULL;
  struct options opt;
  setlocale(LC_ALL,"");
  bindtextdomain(PACKAGE,LOCALEDIR);
  textdomain(PACKAGE);

  if(parse_option(&opt,argc,argv)<0){
	return -1;
  }


  struct commitinfo info;
  int retval;
  git_libgit2_init();
  check_error(git_repository_open(&repo,opt.dir),gettext("open repository"));
  info.b=buf_new();
  get_commitinfo(repo,&info);

  if(opt.action&FILTER_MASK){
	git_repository_free(repo);
	if((opt.action & FILTER_MODE_MASK)==CLEAN)
	  retval= filter(stdin,stdout,filter_clean,NULL);
	else
	  retval= filter(stdin,stdout,filter_expen,&info);
	return retval;
  }
  learning_libgit(repo);
  git_repository_free(repo);
  buf_free(info.b);
}
