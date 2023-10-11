#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

typedef int red_black_tree_key_type;
typedef int red_black_tree_val_type;
typedef struct RedBlackTreeDataNode
{
    red_black_tree_key_type key;
    red_black_tree_val_type val;
} red_black_tree_data_type;
typedef enum
{
    RED = 0,
    BLACK
} red_black_tree_color_type;
typedef struct RedBlackTreeNode
{
    red_black_tree_data_type data;
    red_black_tree_color_type color;
    struct RedBlackTreeNode *parent;
    struct RedBlackTreeNode *left;
    struct RedBlackTreeNode *right;
} red_black_tree_node;
typedef struct RedBlackTree
{
    red_black_tree_node *root;
} red_black_tree;

void red_black_tree_init(red_black_tree *);
int red_black_tree_empty(const red_black_tree *);
red_black_tree_node *red_black_tree_find(red_black_tree *, const red_black_tree_key_type *);
red_black_tree_node *red_black_tree_find_min(red_black_tree *);
red_black_tree_node *red_black_tree_find_max(red_black_tree *);
void red_black_tree_insert(red_black_tree *, const red_black_tree_data_type *);
void red_black_tree_delete(red_black_tree *, const red_black_tree_key_type *);
void red_black_tree_clear(red_black_tree *);

#endif // __RED_BLACK_TREE_H__