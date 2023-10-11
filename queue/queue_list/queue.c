#include "queue.h"
#include <stdlib.h>
#include <assert.h>

static queue_list_node *create_queue_list_node(queue_data_type);

static inline queue_list_node *create_queue_list_node(queue_data_type entry)
{
    queue_list_node *ret = (queue_list_node *)malloc(sizeof(queue_list_node));
    assert(ret != NULL);
    ret->data = entry;
    ret->next = NULL;
    return ret;
}

inline void queue_init(queue *que)
{
    assert(que != NULL);
    que->head = que->tail = NULL;
    que->size = 0;
}

inline unsigned long queue_size(const queue *que)
{
    assert(que != NULL);
    return que->size;
}

inline int queue_empty(const queue *que)
{
    assert(que != NULL);
    return que->size == 0;
}

void queue_push(queue *que, queue_data_type entry)
{
    queue_list_node *new_p = NULL;
    assert(que != NULL);
    new_p = create_queue_list_node(entry);
    if (queue_empty(que))
        que->head = new_p;
    else
        que->tail->next = new_p;
    que->tail = new_p;
    ++que->size;
}

inline queue_data_type queue_front(const queue *que)
{
    assert(que != NULL);
    assert(!queue_empty(que));
    return que->head->data;
}

inline void queue_pop(queue *que)
{
    queue_list_node *tmp = NULL;
    assert(que != NULL);
    assert(!queue_empty(que));
    tmp = que->head->next;
    free(que->head);
    que->head = tmp;
    --que->size;
    if (queue_empty(que))
        que->tail = NULL;
}

void queue_clear(queue *que)
{
    queue_list_node *tmp = NULL;
    assert(que != NULL);
    while (que->head != NULL) {
        tmp = que->head->next;
        free(que->head);
        que->head = tmp;
    }
    que->tail = NULL;
    que->size = 0;
}