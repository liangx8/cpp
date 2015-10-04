#include <malloc.h>
#include <assert.h>
#include "utils.h"
struct _btr_node{
  struct _btr_node *left,*right;
  int llevel,rlevel;
  void *obj;
};

struct _btree{
  int (*cmp)(void *,void*);
  struct _btr_node *root;
};

struct _btr_node* _btr_new_node(void *obj){
  struct _btr_node *node=(struct _btr_node*)malloc(sizeof(struct _btr_node));
  assert(node);
  node->obj=obj;
  node->left=node->right=NULL;
  node->llevel=node->rlevel=0;
  return node;
}
void _recurs_node_free(struct _btr_node *root){
  assert(root);
  if(root->left){
	_recurs_node_free(root->left);
  }
  if(root->right){
	_recurs_node_free(root->right);
  }
  free(root);
}
struct _btr_node* _recurs_node_add(struct _btr_node *root,void *obj,int (*cmp)(void*,void*)){
  assert(cmp);
  if(!root){
 	return _btr_new_node(obj);
  }
  if(cmp(obj,root->obj)<=0){
	root->left=_recurs_node_add(root->left,obj,cmp);
  } else {
	root->right=_recurs_node_add(root->right,obj,cmp);
  }
  return root;
}

btree* btr_new(int (*cmp)(void*,void*)){
  if(cmp){
	btree *b=(btree*)malloc(sizeof(btree));
	b->cmp=cmp;
	b->root=NULL;
	return b;
  }
  return NULL;
								
}
void btr_free(btree *btr){
  if(btr){
	_recurs_node_free(btr->root);
	free(btr);
  }

}
int btr_add(btree *btr,void *obj){
  if(btr){
	btr->root=_recurs_node_add(btr->root,obj,btr->cmp);
	return 0;
  }
  return -1;
}
//int btr_replace(btree *btr,void *n,void **old);

struct _btr_node * _recurs_node_del(struct _btr_node *root,
					 void *obj,
					 void **deleted,
					 int *result,
					 int (*cmp)(void*,void*)){
  if(!root){
	*result=BTREE_DEL_NOT_FOUND;
	return root;
  }
  assert(result);
  int cp=cmp(obj,root->obj);
  if(cp<0){
	root->left=_recurs_node_del(root->left,obj,deleted,result,cmp);
	return root;
  }
  if(cp>0){
	root->right= _recurs_node_del(root->right,obj,deleted,result,cmp);
	return root;
  }
  // found, preform delete
  return NULL;
}
int btr_del(btree *btr,void *obj,void **deleted){
  int retval;
  if(btr){
	btr->root=_recurs_node_del(btr->root,obj,deleted,&retval,btr->cmp);
  }
  return retval;
}
