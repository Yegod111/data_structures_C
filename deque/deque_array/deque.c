#include "deque.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef DEFAULT_DEQUE_CAPACITY
#define DEFAULT_DEQUE_CAPACITY 2
#endif // DEFAULT_DEQUE_CAPACITY

void deque_expand(deque *deq)
{
    assert(deq != NULL);
    if (deq->capacity)
        deq->capacity <<= 1;
    else
        deq->capacity = DEFAULT_DEQUE_CAPACITY;
    deq->data = (deque_data_type *)realloc(deq->data, deq->capacity * sizeof(deque_data_type));
    assert(deq->data != NULL);
    if (deq->front) {
        if (deq->front < (deq->size >> 1)) {
            memcpy(deq->data + deq->size, deq->data, deq->front * sizeof(deque_data_type));
            deq->rear += deq->size;
        } else {
            memcpy(deq->data + deq->front + deq->size, deq->data + deq->front, (deq->size - deq->front) * sizeof(deque_data_type));
            deq->front += deq->size;
        }
    }
}
 
void deque_init(deque *deq, unsigned long init_capacity)
{
    assert(deq != NULL);
    if (init_capacity) {
        deq->data = (deque_data_type *)malloc(init_capacity * sizeof(deque_data_type));
        assert(deq->data != NULL);
    } else {
        deq->data = NULL;
    }
    deq->front = init_capacity >> 1;
    deq->rear = deq->front - 1;
    deq->size = 0;
    deq->capacity = init_capacity;
}
 
inline unsigned long deque_size(const deque *deq)
{
    assert(deq != NULL);
    return deq->size;
}
 
inline unsigned long deque_capacity(const deque *deq)
{
    assert(deq != NULL);
    return deq->capacity;
}
 
inline int deque_empty(const deque *deq)
{
    assert(deq != NULL);
    return deq->size == 0;
}
 
inline int deque_full(const deque *deq)
{
    assert(deq != NULL);
    return deq->size == deq->capacity;
}
 
inline void deque_push_front(deque *deq, deque_data_type entry)
{
    assert(deq != NULL);
    if (deque_full(deq))
        deque_expand(deq);
    if (deq->front == 0)
        deq->front = deq->capacity;
    deq->data[--deq->front] = entry;
    ++deq->size;
}
 
inline void deque_push_back(deque *deq, deque_data_type entry)
{
    assert(deq != NULL);
    if (deque_full(deq))
        deque_expand(deq);
    if (++deq->rear == deq->capacity)
        deq->rear = 0;
    deq->data[deq->rear] = entry;
    ++deq->size;
}
 
inline deque_data_type deque_front(const deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    return deq->data[deq->front];
}
 
inline deque_data_type deque_back(const deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    return deq->data[deq->rear];
}
 
inline void deque_pop_front(deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    if (++deq->front == deq->capacity)
        deq->front = 0;
    --deq->size;
}
 
inline void deque_pop_back(deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    if (deq->rear == 0)
        deq->rear = deq->capacity;
    --deq->rear;
    --deq->size;
}
 
inline void deque_clear(deque *deq)
{
    assert(deq != NULL);
    deq->front = deq->capacity >> 1;
    deq->rear = deq->front - 1;
    deq->size = 0;
}
 
inline void deque_destroy(deque *deq)
{
    assert(deq != NULL);
    if (deq->data != NULL) {
        free(deq->data);
        deq->data = NULL;
        deq->front = 0;
        deq->rear = -1;
        deq->size = deq->capacity = 0;
    }
}
