#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

typedef int avl_tree_key_type;
typedef int avl_tree_val_type;
typedef struct AVLTreeDataNode
{
    avl_tree_key_type key;
    avl_tree_val_type val;
} avl_tree_data_type;
typedef struct AVLTreeNode
{
    avl_tree_data_type data;
    long height;
    struct AVLTreeNode *left;
    struct AVLTreeNode *right;
} avl_tree_node;
typedef struct AVLTree
{
    avl_tree_node *root;
} avl_tree;

void avl_tree_init(avl_tree *);
int avl_tree_empty(const avl_tree *);
avl_tree_node *avl_tree_find(avl_tree *, const avl_tree_key_type *);
avl_tree_node *avl_tree_find_min(avl_tree *);
avl_tree_node *avl_tree_find_max(avl_tree *);
void avl_tree_insert(avl_tree *, const avl_tree_data_type *);
void avl_tree_delete(avl_tree *, const avl_tree_key_type *);
void avl_tree_clear(avl_tree *);

#endif // __AVL_TREE_H__