#include "doubly_list.h"
#include <stdio.h>

void output(const doubly_list *ls);
void output_reverse(const doubly_list *ls);

void output(const doubly_list *ls)
{
    doubly_list_node *p = NULL;
    for (p = ls->head; p != NULL; p = p->next)
        printf("%5d", p->data);
    printf("\n");
}

void output_reverse(const doubly_list *ls)
{
    doubly_list_node *p = NULL;
    for (p = ls->tail; p != NULL; p = p->prev)
        printf("%5d", p->data);
    printf("\n");
}

int main(void)
{
    unsigned long i;
    doubly_list ls;
    doubly_list_init(&ls, 100);
    printf("%u\n", doubly_list_size(&ls));
    output(&ls);
    for (i = 0; i < 100; ++i)
        doubly_list_insert(&ls, i, i + 1);
    output_reverse(&ls);
    for (i = 199; i > 99; --i)
        doubly_list_delete(&ls, i);
    output(&ls);
    doubly_list_clear(&ls);
    return 0;
}