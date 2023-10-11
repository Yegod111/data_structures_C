#include "deque.h"
#include <stdlib.h>
#include <assert.h>

static deque_list_node *create_deque_list_node(deque_data_type);

static inline deque_list_node *create_deque_list_node(deque_data_type entry)
{
    deque_list_node *ret = (deque_list_node *)malloc(sizeof(deque_list_node));
    assert(ret != NULL);
    ret->data = entry;
    ret->prev = ret->next = NULL;
    return ret;
}

inline void deque_init(deque *deq)
{
    assert(deq != NULL);
    deq->head = deq->tail = NULL;
    deq->size = 0;
}

inline unsigned long deque_size(const deque *deq)
{
    assert(deq != NULL);
    return deq->size;
}

inline int deque_empty(const deque *deq)
{
    assert(deq != NULL);
    return deq->size == 0;
}

void deque_push_front(deque *deq, deque_data_type entry)
{
    deque_list_node *new_p = NULL;
    assert(deq != NULL);
    new_p = create_deque_list_node(entry);
    if (deque_empty(deq)) {
        deq->tail = new_p;
    } else {
        deq->head->prev = new_p;
        new_p->next = deq->head;
    }
    deq->head = new_p;
    ++deq->size;
}

void deque_push_back(deque *deq, deque_data_type entry)
{
    deque_list_node *new_p = NULL;
    assert(deq != NULL);
    new_p = create_deque_list_node(entry);
    if (deque_empty(deq)) {
        deq->head = new_p;
    } else {
        deq->tail->next = new_p;
        new_p->prev = deq->tail;
    }
    deq->tail = new_p;
    ++deq->size;
}

inline deque_data_type deque_front(const deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    return deq->head->data;
}

inline deque_data_type deque_back(const deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    return deq->tail->data;
}

void deque_pop_front(deque *deq)
{ 
    assert(deq != NULL);
    assert(!deque_empty(deq));
    if (deq->size == 1) {
        free(deq->head);
        deq->head = deq->tail = NULL;
    } else {
        deq->head = deq->head->next;
        free(deq->head->prev);
        deq->head->prev = NULL;
    }
    --deq->size;
}

void deque_pop_back(deque *deq)
{
    assert(deq != NULL);
    assert(!deque_empty(deq));
    if (deq->size == 1) {
        free(deq->tail);
        deq->head = deq->tail = NULL;
    } else {
        deq->tail = deq->tail->prev;
        free(deq->tail->next);
        deq->tail->next = NULL;
    }
    --deq->size;
}

void deque_clear(deque *deq)
{
    assert(deq != NULL);
    if (deq->head != NULL) {
        while (deq->head != deq->tail) {
            deq->head = deq->head->next;
            free(deq->head->prev);
        }
        free(deq->head);
        deq->head = deq->tail = NULL;
        deq->size = 0;
    }
}