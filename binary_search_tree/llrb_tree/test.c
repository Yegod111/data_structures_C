#include "llrb_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int max(int, int);
int calc_height(llrb_tree_node *);
int count_red(llrb_tree_node *);
int count_black(llrb_tree_node *);
void inorder(llrb_tree_node *);

inline int max(int a, int b)
{
    return a > b ? a : b;
}

int calc_height(llrb_tree_node *root)
{
    return root != NULL ? max(calc_height(root->left), calc_height(root->right)) + 1 : 0;
}

int count_red(llrb_tree_node *root)
{
    return root != NULL ? count_red(root->left) + count_red(root->right) + (root->color == RED) : 0;
}

int count_black(llrb_tree_node *root)
{
    return root != NULL ? count_black(root->left) + count_black(root->right) + (root->color == BLACK) : 1;
}

void inorder(llrb_tree_node *root)
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d\n", root->data.val);
        inorder(root->right);
    }
}

#define MAXN (1 << 22)

int main(void)
{
    int i;
    int key;
    int cnt = 0;
    clock_t begin, end;
    llrb_tree *tree = (llrb_tree *)malloc(sizeof(llrb_tree));
    llrb_tree_init(tree);
    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        llrb_tree_insert(tree, &(llrb_tree_data_type){key, key});
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("height: %d\n", calc_height(tree->root));
    printf("red node: %d\n", count_red(tree->root));
    printf("black node: %d\n", count_black(tree->root));

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < (MAXN >> 1); ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        llrb_tree_delete(tree, &key);
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("height: %d\n", calc_height(tree->root));
    printf("red node: %d\n", count_red(tree->root));
    printf("black node: %d\n", count_black(tree->root));
    
    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        if (i & 1)
            key = i;
        else
            key = rand() + i;
        if (llrb_tree_find(tree, &key) != NULL)
            ++cnt;
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%d\n", cnt);

    llrb_tree_clear(tree);
    free(tree);
    return 0;
}