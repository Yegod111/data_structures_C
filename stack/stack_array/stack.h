#ifndef __STACK_H__
#define __STACK_H__

typedef int stack_data_type;
typedef struct Stack
{
    stack_data_type *data;
    unsigned long size;
    unsigned long capacity;
} stack;

void stack_expand(stack *);
void stack_init(stack *, unsigned long);
unsigned long stack_size(const stack *);
unsigned long stack_capacity(const stack *);
int stack_empty(const stack *);
int stack_full(const stack *);
void stack_push(stack *, stack_data_type);
void stack_pop(stack *);
stack_data_type stack_top(const stack *);
void stack_clear(stack *);
void stack_destroy(stack *);

#endif // __STACK_H__