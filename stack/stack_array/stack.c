#include "stack.h"
#include <stdlib.h>
#include <assert.h>

#ifndef DEFAULT_STACK_CAPACITY
#define DEFAULT_STACK_CAPACITY 2
#endif // DEFAULT_STACK_CAPACITY

inline void stack_expand(stack *stk)
{
    assert(stk != NULL);
    if (stk->capacity)
        stk->capacity <<= 1;
    else
        stk->capacity = DEFAULT_STACK_CAPACITY;
    stk->data = (stack_data_type *)realloc(stk->data, stk->capacity * sizeof(stack_data_type));
    assert(stk->data != NULL);
}

void stack_init(stack *stk, unsigned long init_capacity)
{
    assert(stk != NULL);
    if (init_capacity) {
        stk->data = (stack_data_type *)calloc(init_capacity, sizeof(stack_data_type));
        assert(stk->data != NULL);
    } else {
        stk->data = NULL;
    }
    stk->size = 0;
    stk->capacity = init_capacity;
}

inline unsigned long stack_size(const stack *stk)
{
    assert(stk != NULL);
    return stk->size;
}

inline unsigned long stack_capacity(const stack *stk)
{
    assert(stk != NULL);
    return stk->capacity;
}

inline int stack_empty(const stack *stk)
{
    assert(stk != NULL);
    return stk->size == 0;
}

inline int stack_full(const stack *stk)
{
    assert(stk != NULL);
    return stk->size == stk->capacity;
}

inline void stack_push(stack *stk, stack_data_type entry)
{
    assert(stk != NULL);
    if (stack_full(stk))
        stack_expand(stk);
    stk->data[stk->size++] = entry;
}

inline void stack_pop(stack *stk)
{
    assert(stk != NULL);
    assert(!stack_empty(stk));
    --stk->size;
}

inline stack_data_type stack_top(const stack *stk)
{
    assert(stk != NULL);
    assert(!stack_empty(stk));
    return stk->data[stk->size - 1];
}

inline void stack_clear(stack *stk)
{
    assert(stk != NULL);
    stk->size = 0;
}

void stack_destroy(stack *stk)
{
    assert(stk != NULL);
    if (stk->data != NULL) {
        free(stk->data);
        stk->data = NULL;
        stk->size = stk->capacity = 0;
    }
}