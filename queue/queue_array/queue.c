#include "queue.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef DEFAULT_QUEUE_CAPACITY
#define DEFAULT_QUEUE_CAPACITY 2
#endif // DEFAULT_QUEUE_CAPACITY

void queue_expand(queue *que)
{
    assert(que != NULL);
    if (que->capacity)
        que->capacity <<= 1;
    else
        que->capacity = DEFAULT_QUEUE_CAPACITY;
    que->data = (queue_data_type *)realloc(que->data, que->capacity * sizeof(queue_data_type));
    assert(que->data != NULL);
    if (que->front) {
        if (que->front < (que->size >> 1)) {
            memcpy(que->data + que->size, que->data, que->front * sizeof(queue_data_type));
            que->rear += que->size;
        } else {
            memcpy(que->data + que->size + que->front, que->data + que->front, (que->size - que->front) * sizeof(queue_data_type));
            que->front += que->size;
        }
    }
}

void queue_init(queue *que, unsigned long init_capacity)
{
    assert(que != NULL);
    if (init_capacity) {
        que->data = (queue_data_type *)calloc(init_capacity, sizeof(queue_data_type));
        assert(que->data != NULL);
    } else {
        que->data = NULL;
    }
    que->front = que->rear = 0;
    que->size = 0;
    que->capacity = init_capacity;
}

inline unsigned long queue_size(const queue *que)
{
    assert(que != NULL);
    return que->size;
}

inline unsigned long queue_capacity(const queue *que)
{
    assert(que != NULL);
    return que->capacity;
}

inline int queue_empty(const queue *que)
{
    assert(que != NULL);
    return que->size == 0;
}

inline int queue_full(const queue *que)
{
    assert(que != NULL);
    return que->size == que->capacity;
}

inline void queue_push(queue *que, queue_data_type entry)
{
    assert(que != NULL);
    if (queue_full(que))
        queue_expand(que);
    if (que->rear == que->capacity)
        que->rear = 0;
    que->data[que->rear++] = entry;
    ++que->size;
}

inline queue_data_type queue_front(const queue *que)
{
    assert(que != NULL);
    assert(!queue_empty(que));
    return que->data[que->front];
}

inline void queue_pop(queue *que)
{
    assert(que != NULL);
    assert(!queue_empty(que));
    if (++que->front == que->capacity)
        que->front = 0;
    --que->size;
}

inline void queue_clear(queue *que)
{
    assert(que != NULL);
    que->front = que->rear = 0;
    que->size = 0;
}

inline void queue_destroy(queue *que)
{
    assert(que != NULL);
    if (que->data) {
        free(que->data);
        que->data = 0;
        que->front = que->rear = 0;
        que->size = que->capacity = 0;
    }
}