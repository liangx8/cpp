/*
 * $Id$
 * $Author: wang gang(linux box) <wang.linux.gang@gmail.com>$
 * $Committer: wang gang(linux box) <wang.linux.gang@gmail.com>$
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <locale.h>
#include <git2.h>
#include <libintl.h> // gettext
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>
#include "common.h"
#include "utils.h"

#define LOCALEDIR "locale/"
#define PACKAGE "filter"

#ifndef UNICODE
#define UNICODE
#endif


struct options{
  const char *repo_root_dir;
  const char *ls_path;
};

struct commitinfo{
  const char *c_name,*c_email;
  const char *a_name,*a_email;
  const char *message;
  const char *head_ref_name;
  char strid[GIT_OID_HEXSZ+1];
  string *str;
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
  info->a_name=str_dup(info->str,sig->name);
  info->a_email=str_dup(info->str,sig->email);
  sig=git_commit_committer(commit);
  info->c_name=str_dup(info->str,sig->name);
  info->c_email=str_dup(info->str,sig->email);
  info->message=str_dup(info->str,git_commit_message(commit));
  info->head_ref_name=str_dup(info->str,git_reference_name(head));
  git_oid_tostr(info->strid,sizeof(info->strid),oid);
  git_reference_free(head);
  //  print_commit(commit);
  git_commit_free(commit);
}
static int parse_option(struct options *opt,int argc,char **argv){
  int c;
  opt->ls_path=NULL;
  while((c=getopt(argc,argv,"l:"))!= -1){
	switch (c){
	case 'l':
	  opt->ls_path=optarg;
	  break;
	}
  }
  if(optind >= argc){
	opt->repo_root_dir=find_repo_root(".");
  } else {
	opt->repo_root_dir=find_repo_root(argv[optind]);
  }
  if(!opt->repo_root_dir){
	return -1;
  }
  return 0;
}
void list_history(git_repository *,const char *);
// scan
void scan_all(git_repository *repo);
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

  git_libgit2_init();
  check_error(git_repository_open(&repo,opt.repo_root_dir),gettext("open repository"));
  info.str=str_new();
  get_commitinfo(repo,&info);
  if(opt.ls_path)
	list_history(repo,opt.ls_path);
  else
	list_history(repo,NULL);
  //scan_all(repo);

  git_repository_free(repo);
  str_free(info.str);
}
