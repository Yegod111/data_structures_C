#include "avl_tree.h"
#include <stdlib.h>
#include <assert.h>

static int avl_tree_key_compare(const avl_tree_key_type *, const avl_tree_key_type *);
static int avl_tree_data_compare(const avl_tree_data_type *, const avl_tree_data_type *);
static void avl_tree_key_copy(avl_tree_key_type *, const avl_tree_key_type *);
static void avl_tree_val_copy(avl_tree_val_type *, const avl_tree_val_type *);
static void avl_tree_data_copy(avl_tree_data_type *, const avl_tree_data_type *);
static avl_tree_node *create_avl_tree_node(const avl_tree_data_type *);
static long get_height(const avl_tree_node *);
static long max(long, long);
static long calc_height(avl_tree_node *);
static int get_balance_factor(const avl_tree_node *);
static avl_tree_node *left_rotate(avl_tree_node *);
static avl_tree_node *right_rotate(avl_tree_node *);
static avl_tree_node *left_right_rotate(avl_tree_node *);
static avl_tree_node *right_left_rotate(avl_tree_node *);
static avl_tree_node *avl_tree_balance(avl_tree_node *);
static avl_tree_node *avl_tree_node_find(avl_tree_node *, const avl_tree_key_type *);
static avl_tree_node *find_min_node(avl_tree_node *);
static avl_tree_node *find_max_node(avl_tree_node *);
static avl_tree_node *avl_tree_node_insert(avl_tree_node *, const avl_tree_data_type *);
static avl_tree_node *avl_tree_node_delete(avl_tree_node *, const avl_tree_key_type *);
static avl_tree_node *root_node_delete(avl_tree_node *);
static void avl_tree_node_clear(avl_tree_node *);

static inline int avl_tree_key_compare(const avl_tree_key_type *lhs, const avl_tree_key_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    if (*lhs < *rhs)
        return -1;
    if (*lhs > *rhs)
        return 1;
    return 0;
}

static inline int avl_tree_data_compare(const avl_tree_data_type *lhs, const avl_tree_data_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return avl_tree_key_compare(&lhs->key, &rhs->key);
}

static inline void avl_tree_key_copy(avl_tree_key_type *dest, const avl_tree_key_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void avl_tree_val_copy(avl_tree_val_type *dest, const avl_tree_val_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void avl_tree_data_copy(avl_tree_data_type *dest, const avl_tree_data_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    avl_tree_key_copy(&dest->key, &source->key);
    avl_tree_val_copy(&dest->val, &source->val);
}

static inline avl_tree_node *create_avl_tree_node(const avl_tree_data_type *data_ptr)
{
    assert(data_ptr != NULL);
    avl_tree_node *ret = (avl_tree_node *)malloc(sizeof(avl_tree_node));
    assert(ret != NULL);
    ret->data = *data_ptr;
    ret->height = 0;
    ret->left = ret->right = NULL;
    return ret;
}

static inline long get_height(const avl_tree_node *root)
{
    return root ? root->height : -1;
}

static inline long max(long a, long b)
{
    return a > b ? a : b;
}

static long calc_height(avl_tree_node *root)
{
    return root->height = max(get_height(root->left), get_height(root->right)) + 1;
}

static inline int get_balance_factor(const avl_tree_node *root)
{
    return root ? get_height(root->left) - get_height(root->right) : 0;
}

static inline avl_tree_node *left_rotate(avl_tree_node *root)
{
    assert(root != NULL);
    avl_tree_node *new_root = root->right;
    root->right = new_root->left;
    calc_height(root);
    new_root->left = root;
    calc_height(new_root);
    return new_root;
}

static inline avl_tree_node *right_rotate(avl_tree_node *root)
{
    assert(root != NULL);
    avl_tree_node *new_root = root->left;
    root->left = new_root->right;
    calc_height(root);
    new_root->right = root;
    calc_height(new_root);
    return new_root;
}

static inline avl_tree_node *left_right_rotate(avl_tree_node *root)
{
    assert(root != NULL);
    root->left = left_rotate(root->left);
    return right_rotate(root);
}

static inline avl_tree_node *right_left_rotate(avl_tree_node *root)
{
    assert(root != NULL);
    root->right = right_rotate(root->right);
    return left_rotate(root);
}

inline void avl_tree_init(avl_tree *tree)
{
    assert(tree != NULL);
    tree->root = NULL;
}

inline int avl_tree_empty(const avl_tree *tree)
{
    assert(tree != NULL);
    return tree->root == NULL;
}

static avl_tree_node *avl_tree_node_find(avl_tree_node *root, const avl_tree_key_type *key_ptr)
{
    int cmp;
    assert(key_ptr != NULL);
    while (root != NULL) {
        cmp = avl_tree_key_compare(key_ptr, &root->data.key);
        if (cmp < 0)
            root = root->left;
        else if (cmp > 0)
            root = root->right;
        else
            return root;
    }
    return NULL;
}

inline avl_tree_node *avl_tree_find(avl_tree *tree, const avl_tree_key_type *key_ptr)
{
    assert(tree != NULL);
    assert(key_ptr != NULL);
    return avl_tree_node_find(tree->root, key_ptr);
}

static avl_tree_node *find_min_node(avl_tree_node *root)
{
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

static avl_tree_node *find_max_node(avl_tree_node *root)
{
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

inline avl_tree_node *avl_tree_find_min(avl_tree *tree)
{
    assert(tree != NULL);
    return find_min_node(tree->root);
}

inline avl_tree_node *avl_tree_find_max(avl_tree *tree)
{
    assert(tree != NULL);
    return find_max_node(tree->root);
}

static inline avl_tree_node *avl_tree_balance(avl_tree_node *root)
{
    int bf = get_balance_factor(root);
    if (bf < -1)
        return get_balance_factor(root->right) < 0 ? left_rotate(root) : right_left_rotate(root);
    if (bf > 1)
        return get_balance_factor(root->left) > 0 ? right_rotate(root) : left_right_rotate(root);
    return root;
}

static avl_tree_node *avl_tree_node_insert(avl_tree_node *root, const avl_tree_data_type *data_ptr)
{
    int cmp;
    assert(data_ptr != NULL);
    if (root != NULL) {
        cmp = avl_tree_data_compare(data_ptr, &root->data);
        if (cmp < 0) {
            root->left = avl_tree_node_insert(root->left, data_ptr);
        } else if (cmp > 0) {
            root->right = avl_tree_node_insert(root->right, data_ptr);
        } else {
            avl_tree_val_copy(&root->data.val, &data_ptr->val);
            return root;
        }
        calc_height(root);
        return avl_tree_balance(root);
    }
    return create_avl_tree_node(data_ptr);
}

inline void avl_tree_insert(avl_tree *tree, const avl_tree_data_type *data_ptr)
{
    assert(tree != NULL);
    assert(data_ptr != NULL);
    tree->root = avl_tree_node_insert(tree->root, data_ptr);
}

static avl_tree_node *avl_tree_node_delete(avl_tree_node *root, const avl_tree_key_type *key_ptr)
{
    int cmp;
    assert(key_ptr != NULL);
    if (root != NULL) {
        cmp = avl_tree_key_compare(key_ptr, &root->data.key);
        if (cmp < 0)
            root->left = avl_tree_node_delete(root->left, key_ptr);
        else if (cmp > 0)
            root->right = avl_tree_node_delete(root->right, key_ptr);
        else
            return root_node_delete(root);
        calc_height(root);
        return avl_tree_balance(root);
    }
    return NULL;
}

static avl_tree_node *root_node_delete(avl_tree_node *root)
{
    avl_tree_node *ret = NULL;
    if (root->left != NULL && root->right != NULL) {
        if (get_height(root->left) < get_height(root->right)) {
            avl_tree_data_copy(&root->data, &find_max_node(root->right)->data);
            ret = avl_tree_node_delete(root->right, &root->data.key);
        } else {
            avl_tree_data_copy(&root->data, &find_min_node(root->left)->data);
            ret = avl_tree_node_delete(root->left, &root->data.key);
        }
    } else {
        if (root->left != NULL)
            ret = root->left;
        else if (root->right != NULL)
            ret = root->right;
        else
            ret = NULL;
        free(root);
    }
    return ret;
}

inline void avl_tree_delete(avl_tree *tree, const avl_tree_key_type *key_ptr)
{
    assert(tree != NULL);
    assert(key_ptr != NULL);
    tree->root = avl_tree_node_delete(tree->root, key_ptr);
}

static void avl_tree_node_clear(avl_tree_node *root)
{
    if (root != NULL) {
        avl_tree_node_clear(root->left);
        avl_tree_node_clear(root->right);
        free(root);
    }
}

inline void avl_tree_clear(avl_tree *tree)
{
    assert(tree != NULL);
    avl_tree_node_clear(tree->root);
    tree->root = NULL;
}