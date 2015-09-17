#include <stdio.h>
#include <locale.h>
#include <git2.h>
#include <libintl.h>
#include <errno.h>

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
  const git_signature* committer;
  const git_signature* author;
  const char* message;
};

static void get_commitinfo(git_repository *repo,struct commitinfo *info,const git_oid *id){
  //  print_id(id);
  git_commit *commit;
  check_error(git_commit_lookup(&commit,repo,id),gettext("lookup commit"));
  info->committer=git_commit_author(commit);
  info->author=git_commit_committer(commit);
  info->message=git_commit_message(commit);
  
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
int filter_expen(FILE *out,const char*src,int size,const struct commitinfo *info){
  fprintf(stderr,"expen\n");
  fwrite(src,size,1,out);
  return 0;
}
int filter_clean(FILE *out,const char*src,int size,const struct commitinfo *info){
  fprintf(stderr,"clean\n");
  fwrite(src,size,1,out);
  return 0;
}

int filter(FILE *in,FILE *out,int(*transfer)(FILE*,const char *,int,const struct commitinfo *),const struct commitinfo *info){
  struct bytebuffer buf;
  buf_init(&buf);
  while(1){
	if(transfer){
	  int c=fgetc(in);
	  if(c==EOF)break;
	  buf_add(&buf,c);
	  if(c=='\n'){
		transfer(out,buf.head,buf.size,info);
		buf_reset(&buf);
	  }
	}else{
	  //根据测试，这里还是按单字节看待文件流
	  int c=fgetc(in);
	  if(c==EOF)break;
	  fputc(c,out);
	}
  }
  buf_clean(&buf);
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
	git_libgit2_init();
	check_error(git_repository_open(&repo,repo_root),gettext("open repository"));
	get_commitinfo(repo,&info,get_head_id(repo));
	git_repository_free(repo);
	if(opt.action==CLEAN)
	  return filter(stdin,stdout,filter_clean,NULL);
	//	return filter(stdin,stdout,NULL,&info);
	return filter(stdin,stdout,filter_expen,&info);
  }
  return -1;
}
