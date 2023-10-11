#ifndef __DEQUE_H__
#define __DEQUE_H__

typedef int deque_data_type;
typedef struct Deque
{
    deque_data_type *data;
    unsigned long front;
    unsigned long rear;
    unsigned long size;
    unsigned long capacity;
} deque;

void deque_expand(deque *);
void deque_init(deque *, unsigned long);
unsigned long deque_size(const deque *);
unsigned long deque_capacity(const deque *);
int deque_empty(const deque *);
int deque_full(const deque *);
void deque_push_front(deque *, deque_data_type);
void deque_push_back(deque *, deque_data_type);
deque_data_type deque_front(const deque *);
deque_data_type deque_back(const deque *);
void deque_pop_front(deque *);
void deque_pop_back(deque *);
void deque_clear(deque *);
void deque_destroy(deque *);

#endif // __DEQUE_H__