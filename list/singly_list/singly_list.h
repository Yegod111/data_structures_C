#ifndef __SINGLY_LIST_H__
#define __SINGLY_LIST_H__

typedef int singly_list_data_type;
typedef struct SinglyListNode
{
    singly_list_data_type data;
    struct SinglyListNode *next;
} singly_list_node;
typedef struct SinglyList
{
    singly_list_node *head;
    singly_list_node *tail;
    unsigned long size;
} singly_list;

void singly_list_init(singly_list *, unsigned long);
unsigned long singly_list_size(const singly_list *);
int singly_list_empty(const singly_list *);
singly_list_node *singly_list_get_ptr(const singly_list *, unsigned long);
singly_list_data_type singly_list_get(const singly_list *, unsigned long);
void singly_list_insert(singly_list *, unsigned long, singly_list_data_type);
void singly_list_delete(singly_list *, unsigned long);
void singly_list_assign(singly_list *, unsigned long, singly_list_data_type);
int singly_list_find(const singly_list *, singly_list_data_type);
void singly_list_reverse(singly_list *);
void singly_list_clear(singly_list *);

#endif // __SINGLY_LIST_H__