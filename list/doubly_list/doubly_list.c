#include "doubly_list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static doubly_list_node *create_doubly_list_node(doubly_list_data_type);

static inline doubly_list_node *create_doubly_list_node(doubly_list_data_type entry)
{
    doubly_list_node *ret = (doubly_list_node *)malloc(sizeof(doubly_list_node));
    assert(ret != NULL);
    ret->data = entry;
    ret->prev = ret->next = NULL;
    return ret;
}

void doubly_list_init(doubly_list *ls, unsigned long init_size)
{
    unsigned long i;
    doubly_list_node *new_p = NULL;
    assert(ls != NULL);
    if (init_size) {
        ls->head = ls->tail = create_doubly_list_node(0);
        for (i = 1; i < init_size; ++i) {
            new_p = create_doubly_list_node(0);
            new_p->prev = ls->tail;
            ls->tail->next = new_p;
            ls->tail = new_p;
        }
    } else {
        ls->head = ls->tail = NULL;
    }
    ls->size = init_size;
}

inline unsigned long doubly_list_size(const doubly_list *ls)
{
    assert(ls != NULL);
    return ls->size;
}

inline int doubly_list_empty(const doubly_list *ls)
{
    assert(ls != NULL);
    return ls->size == 0;
}

doubly_list_node *doubly_list_get_ptr(const doubly_list *ls, unsigned long idx)
{
    unsigned long i;
    doubly_list_node *p = NULL;
    assert(ls != NULL && idx < ls->size);
    if (idx < (ls->size >> 1)) {
        p = ls->head;
        for (i = 0; i < idx; ++i)
            p = p->next;
    } else {
        p = ls->tail;
        for (i = ls->size - 1; i > idx; --i)
            p = p->prev;
    }
    return p;
}

inline doubly_list_data_type doubly_list_get(const doubly_list *ls, unsigned long idx)
{
    assert(ls != NULL);
    assert(idx < ls->size);
    return doubly_list_get_ptr(ls, idx)->data;
}

void doubly_list_insert(doubly_list *ls, unsigned long idx, doubly_list_data_type entry)
{
    unsigned long i;
    doubly_list_node *p = NULL;
    doubly_list_node *new_p = NULL;
    assert(ls != NULL);
    assert(idx < ls->size);
    new_p = create_doubly_list_node(entry);
    if (idx == ls->size) {
        if (doubly_list_empty(ls)) {
            ls->head = ls->tail = new_p;
        } else {
            new_p->prev = ls->tail;
            ls->tail->next = new_p;
            ls->tail = new_p;
        }
    } else if (idx == 0) {
        new_p->next = ls->head;
        ls->head->prev = new_p;
        ls->head = new_p;
    } else {
        p = doubly_list_get_ptr(ls, idx);
        new_p->prev = p->prev;
        new_p->next = p;
        p->prev->next = new_p;
        p->prev = new_p;
    }
    ++ls->size;
}

void doubly_list_delete(doubly_list *ls, unsigned long idx)
{
    unsigned long i;
    doubly_list_node *p = NULL;
    assert(ls != NULL && idx < ls->size);
    if (ls->size == 1) {
        free(ls->head);
        ls->head = ls->tail = NULL;
        ls->size = 0;
        return ;
    }
    if (idx == 0) {
        ls->head = ls->head->next;
        free(ls->head->prev);
        ls->head->prev = NULL;
    } else if (idx == ls->size - 1) {
        ls->tail = ls->tail->prev;
        free(ls->tail->next);
        ls->tail->next = NULL;
    } else {
        p = doubly_list_get_ptr(ls, idx);
        printf("%d\n", p->data);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        free(p);
    }
    --ls->size;
}

inline void doubly_list_assign(doubly_list *ls, unsigned long idx, doubly_list_data_type entry)
{
    assert(ls != NULL);
    assert(idx < ls->size);
    doubly_list_get_ptr(ls, idx)->data = entry;
}

int doubly_list_find(const doubly_list *ls, doubly_list_data_type entry)
{
    doubly_list_node *p = NULL;
    assert(ls != NULL);
    if (ls->tail->data == entry)
        return 1;
    for (p = ls->head; p != ls->tail; p = p->next) {
        if (p->data == entry)
            return 1;
    }
    return 0;
}

void doubly_list_clear(doubly_list *ls)
{
    doubly_list_node *p = NULL;
    assert(ls != NULL);
    if (ls->head != NULL) {
        for (p = ls->head->next; p != NULL; p = p->next)
            free(p->prev);
        free(ls->tail);
        ls->head = ls->tail = NULL;
        ls->size = 0;
    }
}
