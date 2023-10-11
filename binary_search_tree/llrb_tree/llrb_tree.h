#ifndef __LLRB_TREE_H__
#define __LLRB_TREE_H__

typedef int llrb_tree_key_type;
typedef int llrb_tree_val_type;
typedef struct LlrbTreeDataNode
{
    llrb_tree_key_type key;
    llrb_tree_val_type val;
} llrb_tree_data_type;
typedef enum
{
    RED = 0,
    BLACK
} llrb_tree_color_type;
typedef struct LlrbTreeNode
{
    llrb_tree_data_type data;
    llrb_tree_color_type color;
    struct LlrbTreeNode *left;
    struct LlrbTreeNode *right;
} llrb_tree_node;
typedef struct LlrbTree
{
    llrb_tree_node *root;
} llrb_tree;

void llrb_tree_init(llrb_tree *);
int llrb_tree_empty(const llrb_tree *);
llrb_tree_node *llrb_tree_find(llrb_tree *, const llrb_tree_key_type *);
llrb_tree_node *llrb_tree_find_min(llrb_tree *);
llrb_tree_node *llrb_tree_find_max(llrb_tree *);
void llrb_tree_insert(llrb_tree *, const llrb_tree_data_type *);
void llrb_tree_delete(llrb_tree *, const llrb_tree_key_type *);
void llrb_tree_clear(llrb_tree *);

#endif // __LLRB_TREE_H__