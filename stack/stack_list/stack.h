#ifndef __STACK_H__
#define __STACK_H__

typedef int stack_data_type;
typedef struct StackListNode
{
    stack_data_type data;
    struct StackListNode *next;
} stack_list_node;
typedef struct Stack
{
    stack_list_node *head;
    unsigned long size;
} stack;

stack_list_node *create_stack_list_node(stack_data_type);
void stack_init(stack *);
unsigned long stack_size(const stack *);
int stack_empty(const stack *);
void stack_push(stack *, stack_data_type);
stack_data_type stack_top(const stack *);
void stack_pop(stack *);
void stack_clear(stack *);

#endif // __STACK_H__