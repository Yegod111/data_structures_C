#include "singly_list.h"
#include <stdio.h>

void output(const singly_list *);

inline void output(const singly_list *ls)
{
    unsigned long i;
    singly_list_node *p = NULL;
    for (p = ls->head; p != NULL; p = p->next)
        printf("%5d", p->data);
    printf("\n");
}

int main(void)
{
    unsigned long i;
    singly_list ls;
    
    singly_list_init(&ls, 10);
    for (i = 10; i < 88; ++i)
        singly_list_insert(&ls, i, i);
    output(&ls);
    singly_list_delete(&ls, 0);
    singly_list_delete(&ls, 50);
    singly_list_delete(&ls, singly_list_size(&ls) - 1);
    output(&ls);

    singly_list_reverse(&ls);
    output(&ls);

    singly_list_clear(&ls);
    singly_list_insert(&ls, 0, 666);
    output(&ls);
    printf("%d\n", singly_list_find(&ls, 666));
    singly_list_delete(&ls, 0);
    singly_list_clear(&ls);
    return 0;
}