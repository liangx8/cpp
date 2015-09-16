#include <stdio.h>
#include <locale.h>
#include <git2.h>
#include <libintl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define LOCALEDIR "locale/"
#define PACKAGE "filter"
#ifndef UNICODE
#define UNICODE
#endif




void check_error(int error,const char *action){
  if(!error){
	return;
  }

  const git_error *e = giterr_last();
  if(e)
	printf("%s %d/%d: %s\n",action,error,e->klass,e->message);
  else
	printf(gettext("Unknown error!!!\n"));
  exit(error);
}
// man opendir
const char *git_dir=".git";
char repo_root[256];
int find_repo_root(){
  DIR *parent=opendir((const char*)repo_root);
  if (!parent){
	printf(gettext("open dir '%s' error:%s\n"),repo_root,strerror(errno));
	return -1;
  }
  struct dirent *child=readdir(parent);
  while(child){
	if((strlen(child->d_name)==4)
	   && (strncmp(git_dir,child->d_name,4)==0)){
	  closedir(parent);
	  return 0;
	}
	child=readdir(parent);
  }
  closedir(parent);
  strcat(repo_root,"/..");
  return find_repo_root();
}
void aindex(git_repository *repo){
  git_index *idx=NULL;
  unsigned int i,ecount;
  char out[GIT_OID_HEXSZ+1];
  out[GIT_OID_HEXSZ]='\0';
  int err=git_repository_index(&idx,repo);
  check_error(err,"read index");
  git_index_read(idx,0);
  ecount=git_index_entrycount(idx);
  if(!ecount){
	printf("空的索引\n");
  }
  for (i=0;i<ecount;i++){
	const git_index_entry *entry=git_index_get_byindex(idx,i);
	git_oid_fmt(out,&entry->id);
	printf(" %s %s\t\n",out,entry->path);

  }
  git_index_free(idx);
}
static void print_time(const git_time *intime, const char *prefix)
{
  char sign, out[32];
  struct tm *intm;
  int offset, hours, minutes;
  time_t t;

  offset = intime->offset;
  if (offset < 0) {
	sign = '-';
	offset = -offset;
  } else {
	sign = '+';
  }

  hours   = offset / 60;
  minutes = offset % 60;

  t = (time_t)intime->time + (intime->offset * 60);

  intm = gmtime(&t);
  strftime(out, sizeof(out), "%a %b %e %T %Y", intm);

  printf("%s%s %c%02d%02d\n", prefix, out, sign, hours, minutes);
}

static void print_commit(git_commit *commit)
{
  char buf[GIT_OID_HEXSZ + 1];
  int i, count;
  const git_signature *sig;
  const char *scan, *eol;

  git_oid_tostr(buf, sizeof(buf), git_commit_id(commit));
  printf("commit %s\n", buf);

  if ((count = (int)git_commit_parentcount(commit)) > 1) {
	printf("Merge:");
	for (i = 0; i < count; ++i) {
	  git_oid_tostr(buf, 8, git_commit_parent_id(commit, i));
	  printf(" %s", buf);
	}
	printf("\n");
  }

  if ((sig = git_commit_author(commit)) != NULL) {
	printf("Author: %s <%s>\n", sig->name, sig->email);
	print_time(&sig->when, "Date:   ");
  }
  printf("\n");

  for (scan = git_commit_message(commit); scan && *scan; ) {
	for (eol = scan; *eol && *eol != '\n'; ++eol) /* find eol */;

	printf("    %.*s\n", (int)(eol - scan), scan);
	scan = *eol ? eol + 1 : NULL;
  }
  printf("\n");
}

void showlog(git_repository *repo){
  git_revwalk *walker;
  git_oid oid;
  int error;
  int ex=0;
  error=git_revwalk_new(&walker,repo);
  check_error(error,gettext("create revsion walker"));
  git_revwalk_sorting(walker,GIT_SORT_REVERSE);

  while(!ex){
	git_commit *commit;
	ex=git_revwalk_next(&oid,walker);
	check_error(error,gettext("get next rev"));
	error=git_commit_lookup(&commit,repo,&oid);
	if(!error){
	  printf(gettext("commit lookup error"));
	  continue;
	}

	print_commit(commit);
	git_commit_free(commit);
  }
}
/*
void show_commit(git_repository *repo){
  git_revwalk walk;
  git_oid oid;
  git_commit *commit=NULL;
}
*/
void show_head(git_repository *repo){
  git_reference *head = NULL;
  int error;
  error = git_repository_head(&head,repo);
  git_reference_free(head);
  check_error(error,gettext("show head"));
  const char *branch=git_reference_shorthand(head);
  printf(gettext("Current branch:\033[32m%s\033[0m\n"),branch);
}
int main(int argc,char **argv){
  setlocale(LC_ALL,"");
  bindtextdomain(PACKAGE,LOCALEDIR);
  textdomain(PACKAGE);
  git_repository *repo=NULL;
  int error;
  repo_root[0]='.';
  repo_root[1]='\0';
  if(!find_repo_root()){
	git_libgit2_init();
	error= git_repository_open(&repo,"./..");
	//error= git_repository_open(&repo,".");
	check_error(error,gettext("open repository"));
	aindex(repo);
	show_head(repo);
	showlog(repo);
	git_repository_free(repo);
  }

}
