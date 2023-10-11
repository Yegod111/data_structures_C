#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

typedef int priority_queue_key_type;
typedef int priority_queue_val_type;
typedef struct PriorityQueueDataNode
{
    priority_queue_key_type key;
    priority_queue_val_type val;
} priority_queue_data_type;
typedef struct PriorityQueue
{
    priority_queue_data_type *data;
    unsigned long capacity;
    unsigned long size;
} priority_queue;

void priority_queue_init(priority_queue *);
unsigned long priority_queue_capacity(const priority_queue *);
unsigned long priority_queue_size(const priority_queue *);
int priority_queue_empty(const priority_queue *);
void priority_queue_push(priority_queue *, const priority_queue_data_type *);
priority_queue_data_type priority_queue_top(const priority_queue *);
void priority_queue_pop(priority_queue *);
void priority_queue_clear(priority_queue *);
void priority_queue_destroy(priority_queue *);

#endif // __PRIORITY_QUEUE_H__