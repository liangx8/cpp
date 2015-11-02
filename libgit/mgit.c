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

#define FILTER_MODE_BIT      0x00
#define CLEAN                0x01
#define EXPEN                0x00
#define FILTER_BIT           0x01
#define LIST_BIT             0x02
#define ORDER_BIT            0x03
#define LIST_HISTORY_BIT     0x04

struct options{
  int action;
  const char *dir;
  int limit;
  const char *list_root;
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
	opt->action |= 1<<FILTER_BIT;
  }

  if(opt->action & (1<<FILTER_BIT)){
	while(1){
	  optc=getopt(argc,argv,"ce");
	  if(optc==-1) break;
	  switch(optc){
	  case 'c':
		//opt->action |= (1<<FILTER_BIT);
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
  opt->dir=NULL;
  opt->limit=20;
  while(1){
	optc=getopt(argc,argv,"p:r:n:l:");
	if(optc==-1)break;
	switch(optc){
	case 'p':
	  opt->dir=find_repo_root(optarg);
	  if(opt->dir==NULL) {
		return -1;
	  }
	  break;
	case 'n':
	  opt->action = (1 << LIST_BIT)+EXPEN;
	  opt->limit=atoi(optarg);
	  break;
	case 'r':
  	  opt->action = (1 << LIST_BIT)+CLEAN;
	  opt->limit=atoi(optarg);
	  break;
	case 'l':
	  opt->action = 1 << LIST_HISTORY_BIT;
	  opt->list_root=optarg;
	  break;
	case '?':
	  printf("%s\n",gettext("wrong options"));
	  exit(-1);
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

struct myobj{
  git_otype otype;
  char strid[GIT_OID_HEXSZ+1];
  size_t size;
};

void list_obj(git_repository *repo,compare cmp,int limit){
  git_odb *odb;
  array *ary=ary_new();

  void obj_free(void *o){
	free(o);
  }
  int each_object(const git_oid *id,void *pl){
	git_odb_object *out;
	check_error(git_odb_read(&out,odb,id),"odb_read");
	struct myobj *mo=(struct myobj *)malloc(sizeof(struct myobj));
	mo->size=git_odb_object_size(out);
	git_oid_tostr(mo->strid,GIT_OID_HEXSZ+1,id);
	mo->otype=git_odb_object_type(out);
	git_odb_object_free(out);
	ary_add(ary,mo);
	return 0;
  }
  check_error(git_repository_odb(&odb,repo),"repository odb");
  git_odb_foreach(odb,each_object,NULL);
  git_odb_free(odb);
  ary_sort(ary,cmp);
  ary_resize(ary,limit,obj_free);
  for(size_t i=0;i<ary_size(ary);i++){
	struct myobj *out;
	ary_get(ary,i,(void **)&out);
	printf("%05ld: %s %5ld %s\n",i,out->strid,out->size,git_object_type2string(out->otype));
	free (out);
  }
  ary_free(ary);
}
int cmp(void *l,void *r){
  struct myobj *lv,*rv;
  lv=(struct myobj*)l;
  rv=(struct myobj*)r;
  return rv->size-lv->size;
}
void learning_libgit(git_repository *);
void list_history(git_repository *,const char *(*)());
writers *wrt;
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

  if(opt.action&(1<<FILTER_BIT)){
	git_repository_free(repo);
	if((opt.action & (1<<FILTER_MODE_BIT))==CLEAN)
	  retval= filter(stdin,stdout,filter_clean,NULL);
	else
	  retval= filter(stdin,stdout,filter_expen,&info);
	return retval;
  }
  //  learning_libgit(repo);
  if(opt.action & (1<< LIST_BIT)){
	if(opt.action & (1<< ORDER_BIT)){
	  list_obj(repo,cmp,opt.limit);
	} else {
	  list_obj(repo,cmp,opt.limit);
	}
  }
  if(opt.action & (1<< LIST_HISTORY_BIT)){
	buffer *b=info.b;
	size_t start=0;
	const char *next(){
	  return buf_split(b,opt.list_root,'/',&start);
	}
	buf_reset(b);
	wrt=wrt_new();
	wrt_add_handler(wrt,stderr);
	wrt_open(wrt);
	list_history(repo,next);
	wrt_close(wrt);
  }
  git_repository_free(repo);
  buf_free(info.b);
}
