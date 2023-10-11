#include "singly_list.h"
#include <stdlib.h>
#include <assert.h>

static singly_list_node *create_singly_list_node(singly_list_data_type);

static inline singly_list_node *create_singly_list_node(singly_list_data_type entry)
{
    singly_list_node *ret = (singly_list_node *)malloc(sizeof(singly_list_node));
    assert(ret != NULL);
    ret->data = entry;
    ret->next = NULL;
    return ret;
}

void singly_list_init(singly_list *ls, unsigned long init_size)
{
    unsigned long i;
    singly_list_node *new_p = NULL;
    assert(ls != NULL);
    if (init_size) {
        ls->head = ls->tail = create_singly_list_node(0);
        for (i = 1; i < init_size; ++i) {
            new_p = create_singly_list_node(0);
            ls->tail->next = new_p;
            ls->tail = new_p;
        }
    } else {
        ls->head = ls->tail = NULL;
    }
    ls->size = init_size;
}

inline unsigned long singly_list_size(const singly_list *ls)
{
    assert(ls != NULL);
    return ls->size;
}

inline int singly_list_empty(const singly_list *ls)
{
    assert(ls != NULL);
    return ls->size == 0;
}

singly_list_node *singly_list_get_ptr(const singly_list *ls, unsigned long idx)
{
    unsigned long i;
    singly_list_node *p = NULL;
    assert(ls != NULL && idx < ls->size);
    if (idx == ls->size - 1)
        return ls->tail;
    p = ls->head;
    for (i = 0; i < idx; ++i)
        p = p->next;
    return p;
}

inline singly_list_data_type single_list_get(const singly_list *ls, unsigned long idx)
{
    assert(ls != NULL);
    assert(idx < ls->size);
    return singly_list_get_ptr(ls, idx)->data;
}

void singly_list_insert(singly_list *ls, unsigned long idx, singly_list_data_type entry)
{
    unsigned long i;
    singly_list_node *p = NULL;
    singly_list_node *new_p = NULL;
    assert(ls != NULL);
    assert(idx <= ls->size);
    new_p = create_singly_list_node(entry);
    if (idx == ls->size) {
        if (singly_list_empty(ls))
            ls->head = new_p;
        else
            ls->tail->next = new_p;
        ls->tail = new_p;
    } else if (idx == 0) {
        new_p->next = ls->head;
        ls->head = new_p;
    } else {
        p = singly_list_get_ptr(ls, idx - 1);
        new_p = create_singly_list_node(entry);
        new_p->next = p->next;
        p->next = new_p;
    }
    ++ls->size;
}

void singly_list_delete(singly_list *ls, unsigned long idx)
{
    unsigned long i;
    singly_list_node dummy_head;
    singly_list_node *p = &dummy_head;
    singly_list_node *tmp = NULL;
    assert(ls != NULL);
    assert(idx < ls->size);
    p->next = ls->head;
    for (i = 0; i < idx; ++i)
        p = p->next;
    tmp = p->next;
    p->next = tmp->next;
    free(tmp);
    ls->head = dummy_head.next;
    --ls->size;
    if (singly_list_empty(ls))
        ls->tail = NULL;
}

inline void singly_list_assign(singly_list *ls, unsigned long idx, singly_list_data_type entry)
{
    assert(ls != NULL);
    assert(idx < ls->size);
    singly_list_get_ptr(ls, idx)->data = entry;
}

int singly_list_find(const singly_list *ls, singly_list_data_type entry)
{
    singly_list_node *p = NULL;
    assert(ls != NULL);
    if (singly_list_empty(ls))
        return 0;
    if (ls->tail->data == entry)
        return 1;
    for (p = ls->head; p != ls->tail; p = p->next) {
        if (p->data == entry)
            return 1;
    }
    return 0;
}

void singly_list_reverse(singly_list *ls)
{
    singly_list_node *pre = NULL;
    singly_list_node *cur = NULL;
    singly_list_node *tmp = NULL;
    assert(ls != NULL);
    pre = NULL;
    cur = ls->head;
    while (cur != NULL) {
        tmp = cur->next;
        cur->next = pre;
        pre = cur;
        cur = tmp;
    }
    ls->tail = ls->head;
    ls->head = pre;
}

void singly_list_clear(singly_list *ls)
{
    singly_list_node *p = NULL;
    singly_list_node *tmp = NULL;
    assert(ls != NULL);
    p = ls->head;
    while (p != NULL) {
        tmp = p->next;
        free(p);
        p = tmp;
    }
    ls->head = ls->tail = NULL;
    ls->size = 0;
}