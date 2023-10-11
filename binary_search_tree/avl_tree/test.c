#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void output(const avl_tree_node *);
void pre_order(const avl_tree_node *);
void in_order(const avl_tree_node *);
void post_order(const avl_tree_node *);
long max(long, long);
long max_height(const avl_tree_node *);


inline void output(const avl_tree_node *root)
{
    printf("key: %10d, val: %10d\n", root->data.key, root->data.val);
}

void pre_order(const avl_tree_node *root)
{
    if (root != NULL) {
        output(root);
        pre_order(root->left);
        pre_order(root->right);
    }
}

void in_order(const avl_tree_node *root)
{
    if (root != NULL) {
        in_order(root->left);
        output(root);
        in_order(root->right);
    }
}

void post_order(const avl_tree_node *root)
{
    if (root != NULL) {
        post_order(root->left);
        post_order(root->right);
        output(root);
    }
}


long max(long a, long b)
{
    return a > b ? a : b;
}

long max_height(const avl_tree_node *root)
{
    return root ? max(max_height(root->left), max_height(root->right)) + 1 : 0;
}

#define MAXN (1 << 22)

int main(void)
{
    int i;
    int key;
    int cnt = 0;
    clock_t begin, end;
    avl_tree *tree = (avl_tree *)malloc(sizeof(avl_tree));
    if (tree == NULL)
        exit(EXIT_FAILURE);
    
    avl_tree_init(tree);
    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        avl_tree_insert(tree, &(avl_tree_data_type){key, key});
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%d\n", tree->root->height);

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < (MAXN >> 1); ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        avl_tree_delete(tree, &key);
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%d\n", tree->root->height);

    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        if (avl_tree_find(tree, &key) != NULL)
            ++cnt;
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%d\n", cnt);

    avl_tree_clear(tree);
    free(tree);
    return 0;
}