#include "red_black_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void preorder(red_black_tree_node *);
void inorder(red_black_tree_node *);
void postorder(red_black_tree_node *);
red_black_tree_key_type generate_key(void);
int count_red_node(red_black_tree_node *);
int count_black_node(red_black_tree_node *);
int max(int, int);
int calc_height(red_black_tree_node *);

void preorder(red_black_tree_node *root)
{
    if (root != NULL) {
        printf("%d(%s)\n", root->data.val, (root->color == RED ? "Red" : "Black"));
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(red_black_tree_node *root)
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d(%s)\n", root->data.val, (root->color == RED ? "Red" : "Black"));
        inorder(root->right);
    }
}

void postorder(red_black_tree_node *root)
{
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d(%s)\n", root->data.val, (root->color == RED ? "Red" : "Black"));
    }
}

inline red_black_tree_key_type generate_key(void)
{
	unsigned int ans = rand();
	ans += ~(ans << 15);
	ans ^= ans >> 10;
	ans += ans << 3;
	ans ^= ans >> 6;
	ans += ~(ans << 11);
	ans ^= ans >> 16;
	return ans;
}

int count_red_node(red_black_tree_node *root)
{
    return root != NULL ? (root->color == RED) + count_red_node(root->left) + count_red_node(root->right) : 0;
}

int count_black_node(red_black_tree_node *root)
{
    return root != NULL ? (root->color == BLACK) + count_black_node(root->left) + count_black_node(root->right) : 1;
}

inline int max(int a, int b)
{
    return a > b ? a : b;
}

int calc_height(red_black_tree_node *root)
{
    return root != NULL ? 1 + max(calc_height(root->left), calc_height(root->right)) : 0;
}

#define MAXN (1 << 22)

int main(void)
{
    int i;
    int key;
    int cnt = 0;
    clock_t begin;
    clock_t end;
    red_black_tree_node *tmp = NULL;
    red_black_tree *tree = (red_black_tree *)malloc(sizeof(red_black_tree));

    red_black_tree_init(tree);
    srand((unsigned int)time(NULL));

    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        red_black_tree_insert(tree, &(red_black_tree_data_type){key, key});
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("red node: %d black node: %d height: %d\n", count_red_node(tree->root), count_black_node(tree->root), calc_height(tree->root));

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < (MAXN >> 1); ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        red_black_tree_delete(tree, &key);
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("red node: %d black node: %d height: %d\n", count_red_node(tree->root), count_black_node(tree->root), calc_height(tree->root));

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        if (red_black_tree_find(tree, &key) != NULL)
            ++cnt;
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%d\n", cnt);
   
    red_black_tree_clear(tree);
    free(tree);
    return 0;
}