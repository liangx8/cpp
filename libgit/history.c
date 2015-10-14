#include <git2.h>
#include <stdio.h>
#include <string.h> // strcmp()
#include <libintl.h> // gettext()
#include "common.h"
#include "utils.h"
struct nodeinfo{
  const char *name;
  git_otype type;
  git_oid oid;
  git_oid cmt_id;
  git_time time;
};
void print_node(struct nodeinfo *p){
  char strid[GIT_OID_HEXSZ+1];
  git_oid_tostr(strid,GIT_OID_HEXSZ+1,&p->oid);
  printf(strid);
  printf(" ");
  git_oid_tostr(strid,10,&p->cmt_id);
  printf(strid);
  printf(" ");
  printf(" %s\n",p->name);

}
/*
void walk_commit(git_repository *repo,const git_oid *id,const char* parent,array *ary){
  git_commit *cmt;
  git_tree *tree;

  const git_signature *author;
  git_time when;
  int treewalk_cb(const char *root,const git_tree_entry *entry,void *payload){
	if(!parent || strcmp(parent,root)==0){
	  struct nodeinfo *node;=malloc(sizeof(struct nodeinfo));
	  struct nodeinfo key;
	  key.name=git_tree_entry_name(entry);
	  ary_add(ary,node);
	  node->name=key.name;
	  node->type=git_tree_entry_type(entry);

	  printf("%s%s",root,node->name);
	  if(node->type==GIT_OBJ_TREE){
		// skip if current entry is tree
		printf("/\n");
	  } else {
		printf("\n");
	  }
	  return 1;
	}
	return 0;
  }

  author=git_commit_author(cmt);
  when.time=author->when.time;
  when.offset=author->when.offset;

  git_tree_walk(tree,GIT_TREEWALK_PRE,treewalk_cb,NULL);
  git_tree_free(tree);
  git_commit_free(cmt);
}
*/
void commit_walk(git_commit *cmt,array *ary){
  git_tree *tree;
  check_error(git_commit_tree(&tree,cmt),gettext("commit tree"));
  int treewalk_cb(const char *root,const git_tree_entry *entry,void *payload){
	const git_oid *eid=git_tree_entry_id(entry);
	int each_cb(void *obj){
	  struct nodeinfo *nd=(struct nodeinfo*)obj;
	  if(git_oid_cmp(&(nd->oid),eid)==0){
		git_oid_cpy(&nd->cmt_id,git_commit_id(cmt));
	  }
	  return 0;
	}
	ary_foreach(ary,each_cb);
	return 0;
  }
  git_tree_walk(tree,GIT_TREEWALK_PRE,treewalk_cb,NULL);
  git_tree_free(tree);

  unsigned int parent_count=git_commit_parentcount(cmt);
  for(unsigned int x=0;x<parent_count;x++){
	git_commit *parent;
	check_error(git_commit_parent(&parent,cmt,x),gettext("commit parent"));
	commit_walk(parent,ary);
	git_commit_free(parent);
  }


}
void list_history(git_repository *repo,const char *(*next)()){
  git_reference *head;
  array *ary=ary_new();
  git_commit *cmt;
  git_tree *tree;
  buffer *buf=buf_new();
  
  const git_oid *cmt_id;
  const char* find_tree(git_tree *tr){
	
	const char *treename=next();
	size_t count;
	count=git_tree_entrycount(tr);
	if(treename==NULL){
	  for(size_t i=0;i<count;i++){
		const git_tree_entry *entry=git_tree_entry_byindex(tr,i);
		struct nodeinfo *node=(struct nodeinfo*)malloc(sizeof(struct nodeinfo));
		ary_add(ary,node);
		git_oid_cpy(&(node->oid),git_tree_entry_id(entry));
		node->name=buf_dup(buf,git_tree_entry_name(entry));
		git_oid_cpy(&(node->cmt_id),cmt_id);
		node->type=git_tree_entry_type(entry);
	  }
	  return NULL;
	}
	for(size_t i=0;i<count;i++){
	  const git_tree_entry *entry=git_tree_entry_byindex(tr,i);
	  const char *name=git_tree_entry_name(entry);
	  if(strcmp(name,treename)==0){
		git_otype type=git_tree_entry_type(entry);
		if(type==GIT_OBJ_TREE){
		  const git_oid *tid=git_tree_entry_id(entry);
		  git_tree *tr1;
		  check_error(git_tree_lookup(&tr1,repo,tid),"tree lookup");
		  const char* retval=find_tree(tr1);
		  git_tree_free(tr1);
		  return retval;
		} else {
		  if(next()){
			// 如果发现找到的entry期望是tree,出错
			return gettext("entry expect a type tree but blob");
		  }
		  struct nodeinfo *node=(struct nodeinfo*)malloc(sizeof(struct nodeinfo));
		  ary_add(ary,node);
		  git_oid_cpy(&(node->oid),git_tree_entry_id(entry));
		  git_oid_cpy(&(node->cmt_id),cmt_id);
		  node->name=buf_dup(buf,git_tree_entry_name(entry));
		  node->type=git_tree_entry_type(entry);


		  return NULL;
		}
	  }
	}
	printf("%s\n",treename);
	return gettext("path not found");
  }

  git_repository_head(&head,repo);
  cmt_id=git_reference_target(head);
  check_error(git_commit_lookup(&cmt,repo,cmt_id),"commit lookup");
  check_error(git_commit_tree(&tree,cmt),"commit tree");
  //  walk_commit(repo,id,NULL,ary);
  const char* msg=find_tree(tree);
  

  git_tree_free(tree);
  if(msg){
	fprintf(stderr,"%s\n",msg);
  } else {
	unsigned int parent_count=git_commit_parentcount(cmt);
	for(unsigned int x=0;x<parent_count;x++){
	  git_commit *parent;
	  check_error(git_commit_parent(&parent,cmt,x),gettext("commit parent"));
	  commit_walk(parent,ary);
	  git_commit_free(parent);
	}
  }

  git_commit_free(cmt);
  git_reference_free(head);
  for(size_t i=0;i<ary_size(ary);i++){
	struct nodeinfo *p;
	ary_get(ary,i,(void**)&p);
	print_node(p);
	free(p);
  }
  ary_free(ary);
  buf_free(buf);
}
