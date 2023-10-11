#include "red_black_tree.h"
#include <stdlib.h>
#include <assert.h>

static int is_red_node(const red_black_tree_node *);
static int is_black_node(const red_black_tree_node *);
static red_black_tree_node *left_rotate(red_black_tree *, red_black_tree_node *);
static red_black_tree_node *right_rotate(red_black_tree *, red_black_tree_node *);
static int red_black_tree_key_compare(const red_black_tree_key_type *, const red_black_tree_key_type *);
static int red_black_tree_data_compare(const red_black_tree_data_type *, const red_black_tree_data_type *);
static void red_black_tree_key_copy(red_black_tree_key_type *, const red_black_tree_key_type *);
static void red_black_tree_val_copy(red_black_tree_val_type *, const red_black_tree_val_type *);
static void red_black_tree_data_copy(red_black_tree_data_type *, const red_black_tree_data_type *);
static red_black_tree_node *create_red_black_tree_node(const red_black_tree_data_type *);
static red_black_tree_node *red_black_tree_node_find(red_black_tree_node *, const red_black_tree_key_type *);
static red_black_tree_node *find_min_node(red_black_tree_node *);
static red_black_tree_node *find_max_node(red_black_tree_node *);
static void fix_up_insertion(red_black_tree *, red_black_tree_node *);
static void fix_up_deletion(red_black_tree *, red_black_tree_node *);
static void red_black_tree_node_clear(red_black_tree_node *);

static inline int is_red_node(const red_black_tree_node *root)
{
    return root != NULL && root->color == RED;
}

static inline int is_black_node(const red_black_tree_node *root)
{
    return root == NULL || root->color == BLACK;
}

static red_black_tree_node *left_rotate(red_black_tree *tree, red_black_tree_node *root)
{
    assert(root != NULL);
    red_black_tree_node *new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    if (root->right != NULL)
        root->right->parent = root;
    if (root->parent == NULL) {
        tree->root = new_root;
    } else {
        if (root->parent->left == root)
            root->parent->left = new_root;
        else
            root->parent->right = new_root;
    }
    new_root->parent = root->parent;
    root->parent = new_root;
    return new_root;
}

static red_black_tree_node *right_rotate(red_black_tree *tree, red_black_tree_node *root)
{
    assert(root != NULL);
    red_black_tree_node *new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    if (root->left != NULL)
        root->left->parent = root;
    if (root->parent == NULL) {
        tree->root = new_root;
    } else {
        if (root->parent->left == root)
            root->parent->left = new_root;
        else
            root->parent->right = new_root;
    }
    new_root->parent = root->parent;
    root->parent = new_root;
    return new_root;
}

inline void red_black_tree_init(red_black_tree *tree)
{
    assert(tree != NULL);
    tree->root = NULL;
}

inline int red_black_tree_empty(const red_black_tree *tree)
{
    assert(tree != NULL);
    return tree->root == NULL;
}

static inline int red_black_tree_key_compare(const red_black_tree_key_type *lhs, const red_black_tree_key_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    if (*lhs < *rhs)
        return -1;
    if (*lhs > *rhs)
        return 1;
    return 0;
}

static inline int red_black_tree_data_compare(const red_black_tree_data_type *lhs, const red_black_tree_data_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return red_black_tree_key_compare(&lhs->key, &rhs->key);
}

static inline void red_black_tree_key_copy(red_black_tree_key_type *dest, const red_black_tree_key_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void red_black_tree_val_copy(red_black_tree_val_type *dest, const red_black_tree_val_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void red_black_tree_data_copy(red_black_tree_data_type *dest, const red_black_tree_data_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    red_black_tree_key_copy(&dest->key, &source->key);
    red_black_tree_val_copy(&dest->val, &source->val);
}

static inline red_black_tree_node *create_red_black_tree_node(const red_black_tree_data_type *data_ptr)
{
    red_black_tree_node *ret = NULL;
    assert(data_ptr != NULL);
    ret = (red_black_tree_node *)malloc(sizeof(red_black_tree_node));
    red_black_tree_data_copy(&ret->data, data_ptr);
    ret->color = RED;
    ret->left = ret->right = ret->parent = NULL;
    return ret;
}

static red_black_tree_node *red_black_tree_node_find(red_black_tree_node *root, const red_black_tree_key_type *key_ptr)
{
    int cmp;
    assert(key_ptr != NULL);
    while (root != NULL) {
        cmp = red_black_tree_key_compare(key_ptr, &root->data.key);
        if (cmp < 0)
            root = root->left;
        else if (cmp > 0)
            root = root->right;
        else
            return root;
    }
    return NULL;
}

inline red_black_tree_node *red_black_tree_find(red_black_tree *tree, const red_black_tree_key_type *key_ptr)
{
    assert(tree != NULL);
    assert(key_ptr != NULL);
    return red_black_tree_node_find(tree->root, key_ptr);
}

static red_black_tree_node *find_min_node(red_black_tree_node *root)
{
    if (root != NULL) {
        while (root->left != NULL)
            root = root->left;
        return root;
    }
    return NULL;
}

inline red_black_tree_node *red_black_tree_find_min(red_black_tree *tree)
{
    assert(tree != NULL);
    return find_min_node(tree->root);
}

static red_black_tree_node *find_max_node(red_black_tree_node *root)
{
    if (root != NULL) {
        while (root->right != NULL)
            root = root->right;
        return root;
    }
    return NULL;
}

inline red_black_tree_node *red_black_tree_find_max(red_black_tree *tree)
{
    assert(tree != NULL);
    return find_max_node(tree->root);
}

void red_black_tree_insert(red_black_tree *tree, const red_black_tree_data_type *data_ptr)
{
    int cmp;
    red_black_tree_node *pre = NULL;
    red_black_tree_node *cur = NULL;
    red_black_tree_node *new_node = NULL;
    assert(tree != NULL);
    assert(data_ptr != NULL);
    cur = tree->root;
    while (cur != NULL) {
        pre = cur;
        cmp = red_black_tree_data_compare(data_ptr, &cur->data);
        if (cmp < 0) {
            cur = cur->left;
        } else if (cmp > 0) {
            cur = cur->right;
        } else {
            red_black_tree_val_copy(&cur->data.val, &data_ptr->val);
            return;
        }
    }
    new_node = create_red_black_tree_node(data_ptr);
    if (pre == NULL) {
        tree->root = new_node; 
    } else {
        new_node->parent = pre;
        if (cmp < 0)
            pre->left = new_node;
        else
            pre->right = new_node;
        fix_up_insertion(tree, new_node);
    }
    tree->root->color = BLACK;
}

static void fix_up_insertion(red_black_tree *tree, red_black_tree_node *root)
{
    red_black_tree_node *parent_node = NULL;
    red_black_tree_node *uncle_node = NULL;
    red_black_tree_node *grandparent_node = NULL;
    assert(root != NULL);
    while (is_red_node(root->parent)) {
        parent_node = root->parent;
        grandparent_node = parent_node->parent;
        grandparent_node->color = RED;
        if (grandparent_node->left == parent_node) {
            uncle_node = grandparent_node->right;
            if (is_red_node(uncle_node)) {
                parent_node->color = uncle_node->color = BLACK;
                root = grandparent_node;
            } else {
                if (parent_node->right == root)
                    root = left_rotate(tree, parent_node)->left;
                right_rotate(tree, grandparent_node)->color = BLACK;
            }
        } else {
            uncle_node = grandparent_node->left;
            if (is_red_node(uncle_node)) {
                parent_node->color = uncle_node->color = BLACK;
                root = grandparent_node;
            } else {
                if (parent_node->left == root)
                    root = right_rotate(tree, parent_node)->right;
                left_rotate(tree, grandparent_node)->color = BLACK;
            }
        }
    }
}

void red_black_tree_delete(red_black_tree *tree, const red_black_tree_key_type *key_ptr)
{
    red_black_tree_node *delete_node = NULL;
    red_black_tree_node *successor = NULL;
    red_black_tree_node *child_node = NULL;
    assert(tree != NULL);
    assert(key_ptr != NULL);
    delete_node = red_black_tree_node_find(tree->root, key_ptr);
    if (delete_node == NULL)
        return;
    if (delete_node->left != NULL && delete_node->right != NULL) {
        successor = find_min_node(delete_node->right);
        red_black_tree_data_copy(&delete_node->data, &successor->data);
        delete_node = successor;
    }
    child_node = delete_node->left != NULL ? delete_node->left : delete_node->right;
    if (child_node != NULL) {
        child_node->parent = delete_node->parent;
        if (delete_node->parent == NULL)
            tree->root = child_node;
        else if (delete_node->parent->left == delete_node)
            delete_node->parent->left = child_node;
        else
            delete_node->parent->right = child_node;
        child_node->color = BLACK;
    } else if (delete_node->parent != NULL) {
        if (is_black_node(delete_node))
            fix_up_deletion(tree, delete_node);
        if (delete_node->parent != NULL)
            delete_node->parent->left = delete_node->parent->right = NULL;
    } else {
        tree->root = NULL;
    }
    free(delete_node);
}

static void fix_up_deletion(red_black_tree *tree, red_black_tree_node *root)
{
    red_black_tree_node *parent_node = NULL;
    red_black_tree_node *brother_node = NULL;
    assert(root != NULL);
    while (root->parent != NULL && is_black_node(root)) {
        parent_node = root->parent;
        if (parent_node->left == root) {
            brother_node = parent_node->right;
            if (is_red_node(brother_node)) {
                parent_node->color = RED;
                brother_node->color = BLACK;
                left_rotate(tree, parent_node);
                brother_node = parent_node->right;
            }
            if (is_black_node(brother_node->right)) {
                brother_node->color = RED;
                if (is_black_node(brother_node->left)) {
                    root = parent_node;
                    continue;
                }
                brother_node->left->color = BLACK;
                brother_node = right_rotate(tree, brother_node);
            }
            brother_node->color = parent_node->color;
            parent_node->color = brother_node->right->color = BLACK;
            left_rotate(tree, parent_node);
        } else {
            brother_node = parent_node->left;
            if (is_red_node(brother_node)) {
                parent_node->color = RED;
                brother_node->color = BLACK;
                right_rotate(tree, parent_node);
                brother_node = parent_node->left;
            }
            if (is_black_node(brother_node->left)) {
                brother_node->color = RED;
                if (is_black_node(brother_node->right)) {
                    root = parent_node;
                    continue;
                }
                brother_node->right->color = BLACK;
                brother_node = left_rotate(tree, brother_node);
            }
            brother_node->color = parent_node->color;
            parent_node->color = brother_node->left->color = BLACK;
            right_rotate(tree, parent_node);
        }
        break;
    }
    root->color = BLACK;
}

static void red_black_tree_node_clear(red_black_tree_node *root)
{
    if (root != NULL) {
        red_black_tree_node_clear(root->left);
        red_black_tree_node_clear(root->right);
        free(root);
    }
}

inline void red_black_tree_clear(red_black_tree *tree)
{
    assert(tree != NULL);
    red_black_tree_node_clear(tree->root);
    tree->root = NULL;
}