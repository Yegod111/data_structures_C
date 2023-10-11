#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int hash_table_key_compare(const hash_table_key_type *, const hash_table_key_type *);
static int hash_table_data_compare(const hash_table_data_type *, const hash_table_data_type *);
static void hash_table_key_copy(hash_table_key_type *, const hash_table_key_type *);
static void hash_table_val_copy(hash_table_val_type *, const hash_table_val_type *);
static void hash_table_data_copy(hash_table_data_type *, const hash_table_data_type *);
static hash_table_list_node *create_hash_table_list_node(const hash_table_data_type *);
static int hash_table_overload(const hash_table *);
static unsigned long hash_table_get_address(const hash_table *, const hash_table_key_type *);
static void hash_table_rehash(hash_table *);

static inline int hash_table_key_compare(const hash_table_key_type *lhs, const hash_table_key_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    if (*lhs < *rhs)
        return -1;
    if (*lhs > *rhs)
        return 1;
    return 0;
}

static inline int hash_table_data_compare(const hash_table_data_type *lhs, const hash_table_data_type *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return hash_table_key_compare(&lhs->key, &rhs->key);
}

static inline void hash_table_key_copy(hash_table_key_type *dest, const hash_table_key_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void hash_table_val_copy(hash_table_val_type *dest, const hash_table_val_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    *dest = *source;
}

static inline void hash_table_data_copy(hash_table_data_type *dest, const hash_table_data_type *source)
{
    assert(dest != NULL);
    assert(source != NULL);
    hash_table_key_copy(&dest->key, &source->key);
    hash_table_val_copy(&dest->val, &source->val);
}

static inline hash_table_list_node *create_hash_table_list_node(const hash_table_data_type *data_ptr)
{
    hash_table_list_node *ret = NULL;
    assert(data_ptr != NULL);
    ret = (hash_table_list_node *)malloc(sizeof(hash_table_list_node));
    assert(ret != NULL);
    hash_table_data_copy(&ret->data, data_ptr);
    ret->next = NULL;
    return ret;
}

static inline int hash_table_overload(const hash_table *ht)
{
    assert(ht != NULL);
    return ht->capacity == 0 || (double)ht->size > (double)ht->capacity * ht->load_factor;
}

inline void hash_table_init(hash_table *ht, double init_load_factor)
{
    assert(ht != NULL);
    ht->hash_list = NULL;
    ht->capacity = ht->size = 0;
    ht->load_factor = init_load_factor;
}

inline unsigned long hash_table_capacity(const hash_table *ht)
{
    assert(ht != NULL);
    return ht->capacity;
}

inline unsigned long hash_table_size(const hash_table *ht)
{
    assert(ht != NULL);
    return ht->size;
}

inline int hash_table_empty(const hash_table *ht)
{
    assert(ht != NULL);
    return ht->size == 0;
}

static inline unsigned long hash_table_get_address(const hash_table *ht, const hash_table_key_type *key_ptr)
{
    assert(ht != NULL);
    assert(key_ptr != NULL);
    return *key_ptr & ht->mask;
}

static void hash_table_rehash(hash_table *ht)
{
    unsigned long i;
    unsigned long addr;
    unsigned long new_capacity;
    hash_table_list_node **new_hash_list = NULL;
    hash_table_list_node *ptr = NULL;
    hash_table_list_node *tmp = NULL;
    assert(ht != NULL);
    if (ht->capacity) {
        new_capacity = ht->capacity << 1;
        new_hash_list = (hash_table_list_node **)calloc(new_capacity, sizeof(hash_table_list_node *));
        assert(new_hash_list != NULL);
        ht->mask = new_capacity - 1;
        for (int i = 0; i < ht->capacity; ++i) {
            ptr = ht->hash_list[i];
            while (ptr != NULL) {
                addr = hash_table_get_address(ht, &ptr->data.key);
                tmp = ptr->next;
                ptr->next = new_hash_list[addr];
                new_hash_list[addr] = ptr;
                ptr = tmp;
            }
        }
        free(ht->hash_list);
        ht->hash_list = new_hash_list;
        ht->capacity = new_capacity;
    } else {
        ht->capacity = 1;
        while ((double)ht->capacity * ht->load_factor < 1)
            ht->capacity <<= 1;
        ht->hash_list = (hash_table_list_node **)calloc(ht->capacity, sizeof(hash_table_list_node *));
        assert(ht->hash_list != NULL);    
        ht->mask = ht->capacity - 1;
    }
}

hash_table_val_type *hash_table_find(hash_table *ht, const hash_table_key_type *key_ptr)
{
    unsigned long addr;
    hash_table_list_node *ptr = NULL;
    assert(ht != NULL);
    assert(key_ptr != NULL);
    addr = hash_table_get_address(ht, key_ptr);
    for (ptr = ht->hash_list[addr]; ptr != NULL; ptr = ptr->next) {
        if (hash_table_key_compare(&ptr->data.key, key_ptr) == 0)
            return &ptr->data.val;
    }
    return NULL;
}

void hash_table_insert(hash_table *ht, const hash_table_data_type *data_ptr)
{
    unsigned long addr;
    hash_table_list_node *ptr = NULL;
    assert(ht != NULL);
    assert(data_ptr != NULL);
    if (hash_table_overload(ht))
        hash_table_rehash(ht);
    addr = hash_table_get_address(ht, &data_ptr->key);
    for (ptr = ht->hash_list[addr]; ptr != NULL; ptr = ptr->next) {
        if (hash_table_key_compare(&ptr->data.key, &data_ptr->key) == 0) {
            hash_table_val_copy(&ptr->data.val, &data_ptr->val);
            return;
        }
    }
    ptr = create_hash_table_list_node(data_ptr);
    ptr->next = ht->hash_list[addr];
    ht->hash_list[addr] = ptr;
    ++ht->size;
}

void hash_table_delete(hash_table *ht, const hash_table_key_type *key_ptr)
{
    unsigned long addr;
    hash_table_list_node dummy_node;
    hash_table_list_node *ptr = &dummy_node;
    hash_table_list_node *tmp = NULL;
    assert(ht != NULL);
    assert(key_ptr != NULL);
    addr = hash_table_get_address(ht, key_ptr);
    ptr->next = ht->hash_list[addr];
    while (ptr->next != NULL) {
        if (hash_table_key_compare(&ptr->next->data.key, key_ptr) == 0) {
            tmp = ptr->next;
            ptr->next = tmp->next;
            free(tmp);
            ht->hash_list[addr] = dummy_node.next;
            --ht->size;
            break;
        }
        ptr = ptr->next;
    }
}

void hash_table_clear(hash_table *ht)
{
    unsigned long i;
    hash_table_list_node *tmp = NULL;
    assert(ht != NULL);
    for (i = 0; i < ht->capacity; ++i) {
        while (ht->hash_list[i] != NULL) {
            tmp = ht->hash_list[i]->next;
            free(ht->hash_list[i]);
            ht->hash_list[i] = tmp;
        }
    }
    ht->size = 0;
}

inline void hash_table_destroy(hash_table *ht)
{
    assert(ht != NULL);
    hash_table_clear(ht);
    free(ht->hash_list);
    ht->hash_list = NULL;
    ht->capacity = 0;
}