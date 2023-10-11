#include "stack.h"
#include <stdlib.h>
#include <assert.h>

inline stack_list_node *create_stack_list_node(stack_data_type entry)
{
    stack_list_node *ret = (stack_list_node *)malloc(sizeof(stack_list_node));
    assert(ret != NULL);
    ret->data = entry;
    ret->next = NULL;
    return ret;
}

inline void stack_init(stack *stk)
{
    assert(stk != NULL);
    stk->head = NULL;
    stk->size = 0;
}

inline unsigned long stack_size(const stack *stk)
{
    assert(stk != NULL);
    return stk->size;
}

inline int stack_empty(const stack *stk)
{
    assert(stk != NULL);
    return stk->size == 0;
}

inline void stack_push(stack *stk, stack_data_type entry)
{
    stack_list_node *new_p = NULL;
    assert(stk != NULL);
    new_p = create_stack_list_node(entry);
    new_p->next = stk->head;
    stk->head = new_p;
    ++stk->size;
}

inline stack_data_type stack_top(const stack *stk)
{
    assert(stk != NULL);
    assert(!stack_empty(stk));
    return stk->head->data;
}

inline void stack_pop(stack *stk)
{
    stack_list_node *tmp = NULL;
    assert(stk != NULL);
    assert(!stack_empty(stk));
    tmp = stk->head->next;
    free(stk->head);
    stk->head = tmp;
    --stk->size;
}

void stack_clear(stack *stk)
{
    stack_list_node *tmp = NULL;
    assert(stk != NULL);
    while (stk->head != NULL) {
        tmp = stk->head->next;
        free(stk->head);
        stk->head = tmp;
    }
    stk->size = 0;
}