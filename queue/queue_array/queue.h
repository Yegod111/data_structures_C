#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef int queue_data_type;
typedef struct Queue
{
    queue_data_type *data;
    unsigned long front;
    unsigned long rear;
    unsigned long size;
    unsigned long capacity;
} queue;

void queue_expand(queue *);
void queue_init(queue *, unsigned long);
unsigned long queue_size(const queue *);
unsigned long queue_capacity(const queue *);
int queue_empty(const queue *);
int queue_full(const queue *);
void queue_push(queue *, queue_data_type);
queue_data_type queue_front(const queue *);
void queue_pop(queue *);
void queue_clear(queue *);
void queue_destroy(queue *);

#endif // __QUEUE_H__