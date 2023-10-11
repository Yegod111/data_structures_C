#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef int queue_data_type;
typedef struct QueueListNode
{
    queue_data_type data;
    struct QueueListNode *next;
} queue_list_node;
typedef struct Queue
{
    queue_list_node *head;
    queue_list_node *tail;
    unsigned long size;
} queue;

void queue_init(queue *);
unsigned long queue_size(const queue *);
int queue_empty(const queue *);
void queue_push(queue *, queue_data_type);
queue_data_type queue_front(const queue *);
void queue_pop(queue *);
void queue_clear(queue *);

#endif // __QUEUE_H__