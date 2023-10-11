#ifndef __DEQUE_H__
#define __DEQUE_H__

typedef int deque_data_type;
typedef struct DequeListNode
{
    deque_data_type data;
    struct DequeListNode *prev;
    struct DequeListNode *next;
} deque_list_node;
typedef struct Deque
{
    deque_list_node *head;
    deque_list_node *tail;
    unsigned long size;
} deque;

void deque_init(deque *);
unsigned long deque_size(const deque *);
int deque_empty(const deque *);
void deque_push_front(deque *, deque_data_type);
void deque_push_back(deque *, deque_data_type);
deque_data_type deque_front(const deque *);
deque_data_type deque_back(const deque *);
void deque_pop_front(deque *);
void deque_pop_back(deque *);
void deque_clear(deque *);

#endif // __DEQUE_H__