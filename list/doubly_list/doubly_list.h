#ifndef __DOUBLY_LIST_H__
#define __DOUBLY_LIST_H__

typedef int doubly_list_data_type;
typedef struct DoublyListNode
{
    doubly_list_data_type data;
    struct DoublyListNode *prev;
    struct DoublyListNode *next;
} doubly_list_node;
typedef struct DoublyList
{
    doubly_list_node *head;
    doubly_list_node *tail;
    unsigned long size;
} doubly_list;

void doubly_list_init(doubly_list *, unsigned long);
unsigned long doubly_list_size(const doubly_list *);
int doubly_list_empty(const doubly_list *);
doubly_list_node *doubly_list_get_ptr(const doubly_list *, unsigned long);
doubly_list_data_type doubly_list_get(const doubly_list *, unsigned long);
void doubly_list_insert(doubly_list *, unsigned long, doubly_list_data_type);
void doubly_list_delete(doubly_list *, unsigned long);
void doubly_list_assign(doubly_list *, unsigned long, doubly_list_data_type);
int doubly_list_find(const doubly_list *, doubly_list_data_type);
void doubly_list_clear(doubly_list *);

#endif // __DOUBLY_LIST_H__