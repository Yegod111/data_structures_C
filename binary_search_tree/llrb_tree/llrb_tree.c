#include "llrb_tree.h"
#include <stdlib.h>
#include <assert.h>

static int llrb_tree_key_compare(const llrb_tree_key_type *, const llrb_tree_key_type *);
static int llrb_tree_data_compare(const llrb_tree_data_type *, const llrb_tree_data_type *);
static void llrb_tree_key_copy(llrb_tree_key_type *, const llrb_tree_key_type *);
static void llrb_tree_val_copy(llrb_tree_val_type *, const llrb_tree_val_type *);
static void llrb_tree_data_copy(llrb_tree_data_type *, const llrb_tree_data_type *);
static int is_red_node(const llrb_tree_node *);
static int is_black_node(const llrb_tree_node *);
static void flip_colors(llrb_tree_node *);
static llrb_tree_node *left_rotate(llrb_tree_node *);
static llrb_tree_node *right_rotate(llrb_tree_node *);
static llrb_tree_node *create_llrb_tree_node(const llrb_tree_data_type *);
static llrb_tree_node *llrb_tree_node_find(llrb_tree_node *, const llrb_tree_key_type *);
static llrb_tree_node *find_min_node(llrb_tree_node *);
static llrb_tree_node *find_max_node(llrb_tree_node *);
static llrb_tree_node *llrb_tree_node_insert(llrb_tree_node *, const llrb_tree_data_type *);
static llrb_tree_node *llrb_tree_balance(llrb_tree_node *);
static llrb_tree_node *borrow_from_right(llrb_tree_node *);
static llrb_tree_node *borrow_from_left(llrb_tree_node *);
static llrb_tree_node *delete_min_node(llrb_tree_node *);
static llrb_tree_node *llrb_tree_delete_node(llrb_tree_node *, const llrb_tree_key_type *);
static void llrb_tree_node_clear(llrb_tree_node *);

static inline int llrb_tree_key_compare(const llrb_tree_key_type *lhs, const llrb_tree_key_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    if (*lhs < *rhs)
        return -1;
    if (*lhs > *rhs)
        return 1;
    return 0;
}

static inline int llrb_tree_data_compare(const llrb_tree_data_type *lhs, const llrb_tree_data_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return llrb_tree_key_compare(&lhs->key, &rhs->key);
}

static inline void llrb_tree_key_copy(llrb_tree_key_type *dest, const llrb_tree_key_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void llrb_tree_val_copy(llrb_tree_val_type *dest, const llrb_tree_val_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void llrb_tree_data_copy(llrb_tree_data_type *dest, const llrb_tree_data_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    llrb_tree_key_copy(&dest->key, &source->key);
    llrb_tree_val_copy(&dest->val, &source->val);
}

static inline int is_red_node(const llrb_tree_node *root)
{
    return root != NULL && root->color == RED;
}

static inline int is_black_node(const llrb_tree_node *root)
{
    return root == NULL || root->color == BLACK;
}

static inline void flip_colors(llrb_tree_node *root)
{
    root->color = (root->color == RED ? BLACK : RED);
    root->left->color = (root->left->color == RED ? BLACK : RED);
    root->right->color = (root->right->color == RED ? BLACK : RED);
}

static inline llrb_tree_node *left_rotate(llrb_tree_node *root)
{
    assert(root != NULL);
    llrb_tree_node *new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    new_root->color = root->color;
    root->color = RED;
    return new_root;
}

static inline llrb_tree_node *right_rotate(llrb_tree_node *root)
{
    assert(root != NULL);
    llrb_tree_node *new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    new_root->color = root->color;
    root->color = RED;
    return new_root;
}

static inline llrb_tree_node *create_llrb_tree_node(const llrb_tree_data_type *data_ptr)
{
    assert(data_ptr != NULL);
    llrb_tree_node *ret = (llrb_tree_node *)malloc(sizeof(llrb_tree_node));
    assert(ret != NULL);
    llrb_tree_data_copy(&ret->data, data_ptr);
    ret->color = RED;
    ret->left = ret->right = NULL;
    return ret;
}

inline void llrb_tree_init(llrb_tree *tree)
{
    assert(tree != NULL);
    tree->root = NULL;
}

inline int llrb_tree_empty(const llrb_tree *tree)
{
    assert(tree != NULL);
    return tree->root == NULL;
}

static llrb_tree_node *llrb_tree_node_find(llrb_tree_node *root, const llrb_tree_key_type *key_ptr)
{
    int cmp;
    assert(key_ptr != NULL);
    while (root != NULL) {
        cmp = llrb_tree_key_compare(key_ptr, &root->data.key);
        if (cmp < 0)
            root = root->left;
        else if (cmp > 0)
            root = root->right;
        else
            return root;
    }
    return NULL;
}

inline llrb_tree_node *llrb_tree_find(llrb_tree *tree, const llrb_tree_key_type *key_ptr)
{
    assert(tree != NULL);
    assert(key_ptr != NULL);
    return llrb_tree_node_find(tree->root, key_ptr);
}

static llrb_tree_node *find_min_node(llrb_tree_node *root)
{
    if (root != NULL) {
        while (root->left != NULL)
            root = root->left;
        return root;
    }
    return NULL;
}

inline llrb_tree_node *llrb_tree_find_min(llrb_tree *tree)
{
    assert(tree != NULL);
    return find_min_node(tree->root);
}

static llrb_tree_node *find_max_node(llrb_tree_node *root)
{
    if (root != NULL) {
        while (root->right != NULL)
            root = root->right;
        return root;
    }
    return NULL;
}

inline llrb_tree_node *llrb_tree_find_max(llrb_tree *tree)
{
    assert(tree != NULL);
    return find_max_node(tree->root);
}

static llrb_tree_node *llrb_tree_node_insert(llrb_tree_node *root, const llrb_tree_data_type *data_ptr)
{
    int cmp;
    if (root != NULL) {
        cmp = llrb_tree_data_compare(data_ptr, &root->data);
        if (cmp < 0) {
            root->left = llrb_tree_node_insert(root->left, data_ptr);
        } else if (cmp > 0) {
            root->right = llrb_tree_node_insert(root->right, data_ptr);
        } else {
            llrb_tree_val_copy(&root->data.val, &data_ptr->val);
            return root;
        }
        return llrb_tree_balance(root);
    }
    return create_llrb_tree_node(data_ptr);
}

static inline llrb_tree_node *llrb_tree_balance(llrb_tree_node *root)
{
    if (is_black_node(root->left) && is_red_node(root->right))
        root = left_rotate(root);
    if (is_red_node(root->left) && is_red_node(root->left->left))
        root = right_rotate(root);
    if (is_red_node(root->left) && is_red_node(root->right))
        flip_colors(root);
    return root;
}

inline void llrb_tree_insert(llrb_tree *tree, const llrb_tree_data_type *data_ptr)
{
    assert(tree != NULL);
    assert(data_ptr != NULL);
    tree->root = llrb_tree_node_insert(tree->root, data_ptr);
    tree->root->color = BLACK;
}

static inline llrb_tree_node *borrow_from_right(llrb_tree_node *root)
{
    flip_colors(root);
    if (is_red_node(root->right->left)) {
        root->right = right_rotate(root->right);
        root = left_rotate(root);
        flip_colors(root);
    }
    return root;
}

static inline llrb_tree_node *borrow_from_left(llrb_tree_node *root)
{
    flip_colors(root);
    if (is_red_node(root->left->left)) {
        root = right_rotate(root);
        flip_colors(root);
    }
    return root;
}

static llrb_tree_node *delete_min_node(llrb_tree_node *root)
{
    if (root->left == NULL) {
        free(root);
        return NULL;
    }
    if (is_black_node(root->left) && is_black_node(root->left->left))
        root = borrow_from_right(root);
    root->left = delete_min_node(root->left);
    return llrb_tree_balance(root);
}

void llrb_tree_delete(llrb_tree *tree, const llrb_tree_key_type *key_ptr)
{
    assert(tree != NULL);
    assert(key_ptr != NULL);
    if (llrb_tree_empty(tree))
        return;
    if (is_black_node(tree->root->left) && is_black_node(tree->root->right))
        tree->root->color = RED;
    tree->root = llrb_tree_delete_node(tree->root, key_ptr);
    if (is_red_node(tree->root))
        tree->root->color = BLACK;
}

static llrb_tree_node *llrb_tree_delete_node(llrb_tree_node *root, const llrb_tree_key_type *key_ptr)
{
    llrb_tree_node *tmp = NULL;
    if (root == NULL)
        return NULL;
    if (llrb_tree_key_compare(key_ptr, &root->data.key) < 0) {
        if (root->left == NULL)
            return root;
        if (is_black_node(root->left) && is_black_node(root->left->left))
            root = borrow_from_right(root);
        root->left = llrb_tree_delete_node(root->left, key_ptr);
    } else {
        if (is_red_node(root->left))
            root = right_rotate(root);
        if (root->right == NULL) {
            if (llrb_tree_key_compare(key_ptr, &root->data.key) == 0) {
                free(root);
                return NULL;
            }
            return root;
        }
        if (is_black_node(root->right) && is_black_node(root->right->left))
            root = borrow_from_left(root);
        if (llrb_tree_key_compare(key_ptr, &root->data.key) == 0) {
            tmp = find_min_node(root->right);
            llrb_tree_data_copy(&root->data, &tmp->data);
            root->right = delete_min_node(root->right);
        } else {
            root->right = llrb_tree_delete_node(root->right, key_ptr);
        }
    }
    return llrb_tree_balance(root);
}

static void llrb_tree_node_clear(llrb_tree_node *root)
{
    if (root != NULL) {
        llrb_tree_node_clear(root->left);
        llrb_tree_node_clear(root->right);
        free(root);
    }
}

inline void llrb_tree_clear(llrb_tree *tree)
{
    assert(tree != NULL);
    llrb_tree_node_clear(tree->root);
    tree->root = NULL;
}