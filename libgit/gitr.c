#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <git2.h>
void check_error(int error,const wchar_t *action){
  if(!error){
	return;
  }

  const git_error *e = giterr_last();
  if(e)
	wprintf(L"%s %d/%d: %s\n",action,error,e->klass,e->message);
  else
	wprintf(L"Unknown error!!!\n");
  exit(error);
}

void aindex(git_repository *repo){
  git_index *idx=NULL;
  int err=git_repository_index(&idx,repo);
  check_error(err,L"read index");
  git_tree *tree=NULL;
  err=git_revparse_single((git_object**)&tree,repo,"HEAD~^{tree}");
  check_error(err,L"revparse");
  err=git_index_read_tree(idx,tree);
  check_error(err,L"read tree");
  if (tree){
	wprintf(L"index object:0x%x\n",tree);
  }
}
int main(int argc,char **argv){
  setlocale(LC_ALL,"");
  git_repository *repo=NULL;
  int error;
  git_libgit2_init();
  error= git_repository_open(&repo,"/home/arm/git/g");
  //error= git_repository_open(&repo,".");
  check_error(error,L"open repository");

  aindex(repo);
  git_repository_free(repo);
  wprintf(L"hello\n");
}
