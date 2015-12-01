#include <libintl.h> // gettext
#include <git2.h>
#include <stdio.h>
#include "common.h"
#include "utils.h"
//string *str;
auto_buffer *blob_buf,*entry_buf,*tree_buf,*commit_buf,*tag_buf;
array *blobs,*trees,*commits,*tags;
//git_repository *repo;

void show_id(const git_oid *id){
  char strid[GIT_OID_HEXSZ+1];
  git_oid_tostr(strid,GIT_OID_HEXSZ+1,id);
  printf(strid);
}
struct obj_head{
  const git_oid *id;
  git_otype type;
};
struct blob{
  struct obj_head head; // must be first
};
struct tree{
  struct obj_head head; // must be first
  array *entries;
};
struct commit{
  struct obj_head head; // must be first

  struct tree *tr;
  
};
struct tag{
  struct obj_head head; // must be first
};
struct entry{
  const struct blob *b;
  const char *name;
};
void scan_init(){
  //  str=str_new();
  blob_buf=ab_new(sizeof(struct blob));

  entry_buf=ab_new(sizeof(struct entry));
  tree_buf=ab_new(sizeof(struct tree));
  commit_buf=ab_new(sizeof(struct commit));
  tag_buf=ab_new(sizeof(struct tag));
  blobs=ary_new();
  //  ary_sort(blobs,cmp);
  trees=ary_new();
  //  ary_sort(trees,cmp);
  commits=ary_new();
  //  ary_sort(commits,cmp);
  tags=ary_new();
}
void scan_free(){
  //  str_free(str);
  ab_free(blob_buf);
  ab_free(entry_buf);
  ab_free(tree_buf);
  ab_free(commit_buf);
  ary_free(blobs);
  ary_free(trees);
  ary_free(commits);
}
// return 1 if not exists and add, otherwise return 0
void add_obj(array *ary,auto_buffer *ab,const git_oid *id,git_otype type){
  struct obj_head *b=ab_obj_malloc(ab);
  b->id=id;
  b->type=type;
  ary_add(ary,b);
}

int all_objects(const git_oid *oid,void *payload){
  git_odb *odb=(git_odb *)payload;

  git_odb_object *obj;
  git_otype type;

  git_odb_read(&obj,odb,oid);
  type=git_odb_object_type(obj);
  //  printf("%s %s\n",strid,git_object_type2string(type));
  switch(type){
  case GIT_OBJ_COMMIT:
	add_obj(commits,commit_buf,oid,GIT_OBJ_COMMIT);
	break;
  case GIT_OBJ_TREE:
	add_obj(trees,tree_buf,oid,GIT_OBJ_TREE);
	break;
  case GIT_OBJ_BLOB:
	add_obj(blobs,blob_buf,oid,GIT_OBJ_BLOB);
	break;
  case GIT_OBJ_TAG:
	add_obj(tags,tag_buf,oid,GIT_OBJ_TAG);
	break;
  default:;
  }

  return 0;
}
void scan_all(git_repository *repo){
  string *str=str_new();
/*
 * 函数嵌套的有效范围只是在当前函数中.当当前函数结束,嵌套函数就无效
 */
  int cmp(void *l,void *r){
	struct obj_head *ol=(struct obj_head*)l;
	struct obj_head *or=(struct obj_head*)r;
	return git_oid_cmp(ol->id,or->id);
  }
  int show(void *obj){
	char strid[GIT_OID_HEXSZ+1];
	struct obj_head *h=(struct obj_head*)obj;
	git_oid_tostr(strid,GIT_OID_HEXSZ+1,h->id);
	printf("%s %s\n",strid,git_object_type2string(h->type));
	return 0;
  }

  int commit_info(void *obj){
	char strid[8];
	struct commit *scmt=(struct commit *)obj;
	git_commit *cmt;
	git_tree *gtr;
	struct tree *tr;
	struct obj_head key;
	if(git_commit_lookup(&cmt,repo,scmt->head.id)){
	  return 0;
	}
	git_oid_tostr(strid,8,scmt->head.id);
	printf("%s ",strid);
	check_error(git_commit_tree(&gtr,cmt),gettext("git_commit_tree"));
	key.id=git_tree_id(gtr);
	git_oid_tostr(strid,8,key.id);
	printf("%s \n",strid);
	if(ary_find(trees,&key,(void **)&tr,NULL)){
	  printf("tree not found, and error exit\n");
	  return -1;
	}
	scmt->tr=tr;
	git_commit_free(cmt);

	return 0;
  }
  //  repo=rpo;
  git_odb *odb;
  scan_init();
  check_error(git_repository_odb(&odb,repo),gettext("repository odb"));
  git_odb_foreach(odb,all_objects,odb);
  ary_sort(blobs,cmp);
  ary_sort(trees,cmp);
  ary_foreach(commits,commit_info);
  ary_foreach(trees,show);
  git_odb_free(odb);
  scan_free();
  str_free(str);
}
