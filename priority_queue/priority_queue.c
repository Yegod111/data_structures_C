#include "priority_queue.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef DEFAULT_PRIORITY_QUEUE_CAPACITY
#define DEFAULT_PRIORITY_QUEUE_CAPACITY 1
#endif // DEFAULT_PRIORITY_QUEUE_CAPACITY

static int priority_queue_key_compare(const priority_queue_key_type *, const priority_queue_key_type *);
static int priority_queue_data_compare(const priority_queue_data_type *, const priority_queue_data_type *);
static void priority_queue_key_copy(priority_queue_key_type *, const priority_queue_key_type *);
static void priority_queue_val_copy(priority_queue_val_type *, const priority_queue_val_type *);
static void priority_queue_data_copy(priority_queue_data_type *, const priority_queue_data_type *);
static int priority_queue_full(const priority_queue *);
static void priority_queue_expand(priority_queue *);
static void priority_queue_data_swap(priority_queue_data_type *, priority_queue_data_type *);
static void sift_up(priority_queue *);
static void sift_down(priority_queue *);

static inline int priority_queue_key_compare(const priority_queue_key_type *lhs, const priority_queue_val_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    if (*lhs < *rhs)
        return -1;
    if (*lhs > *rhs)
        return 1;
    return 0;
}

static inline int priority_queue_data_compare(const priority_queue_data_type *lhs, const priority_queue_data_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return priority_queue_key_compare(&lhs->key, &rhs->key);
}

static inline void priority_queue_key_copy(priority_queue_key_type *dest, const priority_queue_key_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void priority_queue_val_copy(priority_queue_val_type *dest, const priority_queue_val_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void priority_queue_data_copy(priority_queue_data_type *dest, const priority_queue_data_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    priority_queue_key_copy(&dest->key, &source->key);
    priority_queue_val_copy(&dest->val, &source->val);
}

static inline int priority_queue_full(const priority_queue *que)
{
    assert(que != NULL);
    return que->size == que->capacity;
}

static inline void priority_queue_expand(priority_queue *que)
{
    assert(que != NULL);
    if (que->capacity)
        que->capacity <<= 1;
    else
        que->capacity = DEFAULT_PRIORITY_QUEUE_CAPACITY;
    que->data = (priority_queue_data_type *)realloc(que->data, que->capacity * sizeof(priority_queue_data_type));
    assert(que->data);
}

static inline void priority_queue_data_swap(priority_queue_data_type *lhs, priority_queue_data_type *rhs)
{
    priority_queue_data_type tmp;
    assert(lhs != NULL);
    assert(rhs != NULL);
    priority_queue_data_copy(&tmp, lhs);
    priority_queue_data_copy(lhs, rhs);
    priority_queue_data_copy(rhs, &tmp);
}

static void sift_up(priority_queue *que)
{
    unsigned long child = que->size - 1;
    unsigned long parent = (child - 1) >> 1;
    priority_queue_data_type target;
    assert(que != NULL);
    priority_queue_data_copy(&target, &que->data[child]);
    while (child != 0) {
        if (priority_queue_data_compare(&target, &que->data[parent]) <= 0)
            break;
        priority_queue_data_copy(&que->data[child], &que->data[parent]);
        child = parent;
        parent = (child - 1) >> 1;
    }
    priority_queue_data_copy(&que->data[child], &target);
}

static void sift_down(priority_queue *que)
{
    unsigned long parent = 0;
    unsigned long child = parent << 1 | 1;
    priority_queue_data_type target;
    assert(que != NULL);
    priority_queue_data_copy(&target, &que->data[parent]);
    while (child < que->size) {
        if (child + 1 < que->size && priority_queue_data_compare(&que->data[child + 1], &que->data[child]) > 0)
            ++child;
        if (priority_queue_data_compare(&target, &que->data[child]) >= 0)
            break;
        priority_queue_data_copy(&que->data[parent], &que->data[child]);
        parent = child;
        child = parent << 1 | 1;
    }
    priority_queue_data_copy(&que->data[parent], &target);
}

inline void priority_queue_init(priority_queue *que)
{
    assert(que != NULL);
    que->data = NULL;
    que->size = que->capacity = 0;
}

inline unsigned long priority_queue_capacity(const priority_queue *que)
{
    assert(que != NULL);
    return que->capacity;
}

inline unsigned long priority_queue_size(const priority_queue *que)
{
    assert(que != NULL);
    return que->size;
}

inline int priority_queue_empty(const priority_queue *que)
{
    assert(que != NULL);
    return que->size == 0;
}

inline void priority_queue_push(priority_queue *que, const priority_queue_data_type *data_ptr)
{
    assert(que != NULL);
    assert(data_ptr != NULL);
    if (priority_queue_full(que))
        priority_queue_expand(que);
    priority_queue_data_copy(&que->data[que->size], data_ptr);
    ++que->size;
    sift_up(que);
}

inline priority_queue_data_type priority_queue_top(const priority_queue *que)
{
    assert(que != NULL);
    assert(!priority_queue_empty(que));
    return que->data[0];
}

inline void priority_queue_pop(priority_queue *que)
{
    assert(que != NULL);
    assert(!priority_queue_empty(que));
    --que->size;
    priority_queue_data_swap(&que->data[0], &que->data[que->size]);
    sift_down(que);
}

inline void priority_queue_clear(priority_queue *que)
{
    assert(que != NULL);
    que->size = 0;
}

inline void priority_queue_destroy(priority_queue *que)
{
    assert(que != NULL);
    free(que->data);
    que->data = NULL;
    que->size = que->capacity = 0;
}