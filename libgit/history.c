#include <git2.h>
#include <stdio.h>
#include <string.h> // strcmp()
#include <libintl.h> // gettext()
#include <time.h>
#include "common.h"
#include "mem.h"
#include "utils.h"
struct nodeinfo{
  const char *name;
  git_otype type;
  git_oid oid;
  git_oid cmt_id;
  git_time atime;//author time
  git_time ctime;//committer time
  char msg[50];
};

void msg_cpy(char msg[],const char*src,int len){
  int sz=strlen(src);
  strncpy(msg,src,len-1);
  if (sz > len-1){
	msg[len-4]=msg[len-3]=msg[len-2]='.';
	msg[len-1]='\0';
  } else {
	msg[sz]=0;
  }
  for(int i=0;i<len;i++){
	if(msg[i]=='\n')msg[i]=' ';
  }
}

void print_node(struct nodeinfo *p){
  char strid[GIT_OID_HEXSZ+1];
  char tout[32];
  char name[20];
  struct tm time;
  char sign;
  int offset;
  if(p->atime.offset<0){
	sign='-';
	offset=- p->atime.offset;
  }else {
	sign='+';
	offset=p->atime.offset;
  }
  msg_cpy(name,p->name,20);
  printf("%-20s",name);
  gmtime_r(&p->atime.time,&time);
  strftime(tout,32,"%F %T %Z",&time);
  git_oid_tostr(strid,10,&p->oid);
  printf(" %s ",strid);
  git_oid_tostr(strid,10,&p->cmt_id);
  printf("\033[33m%s\033[0m ",strid);
  printf(tout);
  printf(" %c%02d%02d",sign,offset/60,offset%60);
  printf(" %s\n",p->msg);

}
int time_cmp(const git_time *left,const git_time *right){
  return left->time+left->offset-right->time-right->offset;
}
void commit_walk(git_commit *cmt,array *nodes,int (*cmt_bag)(const git_oid *)){
  git_tree *tree;
  const git_oid *cmt_id=git_commit_id(cmt);
  const git_signature *author,*committer;
  char strid[GIT_OID_HEXSZ+1];
  unsigned int parent_count=git_commit_parentcount(cmt);
  git_oid_tostr(strid,12,cmt_id);
  if(!cmt_bag(cmt_id))return;
  //  getchar();

  //  printf("%s:%5d: %s %s\n",__FILE__,__LINE__,strid,git_commit_message(cmt));
  //  printf("%s:%5d: parent count:%d\n",__FILE__,__LINE__,parent_count);
  check_error(git_commit_tree(&tree,cmt),gettext("commit tree"));
  int treewalk_cb(const char *root,const git_tree_entry *entry,void *payload){
	const git_oid *eid=git_tree_entry_id(entry);
	int each_cb(void *obj){
	  struct nodeinfo *nd=(struct nodeinfo*)obj;

	  if(git_oid_cmp(&(nd->oid),eid)==0 && time_cmp(&(nd->atime),&(author->when))>0){
		git_oid_cpy(&(nd->cmt_id),cmt_id);
		nd->atime.time=author->when.time;
		nd->atime.offset=author->when.offset;
		nd->ctime.time=committer->when.time;
		nd->ctime.offset=committer->when.offset;
		msg_cpy(nd->msg,git_commit_message(cmt),50);
	  }
	  return 0;
	}
	ary_foreach(nodes,each_cb);
	return 0;
  }
  author=git_commit_author(cmt);
  committer=git_commit_committer(cmt);
  git_tree_walk(tree,GIT_TREEWALK_PRE,treewalk_cb,NULL);
  git_tree_free(tree);

  
  for(unsigned int x=0;x<parent_count;x++){
	git_commit *parent;
	check_error(git_commit_parent(&parent,cmt,x),gettext("commit parent"));
	commit_walk(parent,nodes,cmt_bag);
	git_commit_free(parent);
  }

}
void list_history(git_repository *repo,const char *path){
  git_reference *head;
  array *nodes=ary_new();
  array *ary_cmt=ary_new();
  git_commit *cmt;
  const git_signature *author,*committer;
  git_tree *tree;
  string *str=str_new();
  auto_buffer *oidbuf=ab_new(sizeof(git_oid));
  auto_buffer *nibuf=ab_new(sizeof(struct nodeinfo));
  
  const git_oid *cmt_id;
  int cmt_cmp(void *l,void *r){
	return git_oid_cmp((const git_oid*)l,(const git_oid*)r);
  }
  ary_sort(ary_cmt,cmt_cmp);
  int cmt_bag(const git_oid* id){
	git_oid *oid=(git_oid*)ab_obj_malloc(oidbuf);
	git_oid_cpy(oid,id);
	if(ary_find(ary_cmt,(void *)id,NULL,NULL)){
	  ary_add(ary_cmt,(void *)oid);
	  return -1;
	}
	ab_obj_free(oidbuf,oid);
	return 0;
  }
  const char* find_tree(git_tree *tr){
	size_t count;
	count=git_tree_entrycount(tr);
	for(size_t i=0;i<count;i++){
	  const git_tree_entry *entry=git_tree_entry_byindex(tr,i);
	  struct nodeinfo *node=ab_obj_malloc(nibuf);
	  ary_add(nodes,node);
	  git_oid_cpy(&(node->oid),git_tree_entry_id(entry));
	  node->name=str_dup(str,git_tree_entry_name(entry));
	  git_oid_cpy(&(node->cmt_id),cmt_id);
	  node->type=git_tree_entry_type(entry);
	  node->atime.time=author->when.time;
	  node->atime.offset=author->when.offset;
	  node->ctime.time=committer->when.time;
	  node->ctime.offset=committer->when.offset;
	  msg_cpy(node->msg,git_commit_message(cmt),50);
	}
	return NULL;
  }

  git_repository_head(&head,repo);
  cmt_id=git_reference_target(head);
  check_error(git_commit_lookup(&cmt,repo,cmt_id),"commit lookup");
  check_error(git_commit_tree(&tree,cmt),"commit tree");
  author=git_commit_author(cmt);
  committer=git_commit_committer(cmt);

  const char* msg=find_tree(tree);
  

  git_tree_free(tree);
  if(msg){
	fprintf(stderr,"%s\n",msg);
  } else {
	unsigned int parent_count=git_commit_parentcount(cmt);
	for(unsigned int x=0;x<parent_count;x++){
	  git_commit *parent;
	  check_error(git_commit_parent(&parent,cmt,x),gettext("commit parent"));
	  commit_walk(parent,nodes,cmt_bag);
	  git_commit_free(parent);
	}
  }

  git_commit_free(cmt);
  git_reference_free(head);
  for(size_t i=0;i<ary_size(nodes);i++){
	struct nodeinfo *p;
	ary_get(nodes,i,(void**)&p);
	print_node(p);
  }
  ary_free(ary_cmt);
  ary_free(nodes);
  str_free(str);
  ab_free(oidbuf);
  ab_free(nibuf);
}
