#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int hash_table_key_compare(const hash_table_key_type *, const hash_table_key_type *);
static int hash_table_data_compare(const hash_table_data_type *, const hash_table_data_type *);
static void hash_table_key_copy(hash_table_key_type *, const hash_table_key_type *);
static void hash_table_val_copy(hash_table_val_type *, const hash_table_val_type *);
static void hash_table_data_copy(hash_table_data_type *, const hash_table_data_type *);
static int hash_table_overload(const hash_table *);
static unsigned long hash_table_get_address(const hash_table *, const hash_table_key_type *);
static unsigned long linear_probing(const hash_table *, unsigned long);
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
    hash_table_val_copy(&dest->val, &dest->val);
}

inline void hash_table_init(hash_table *ht, double init_load_factor)
{
    assert(ht != NULL);
    assert(init_load_factor > 0);
    assert(init_load_factor < 2);
    ht->data = NULL;
    ht->occupation = NULL;
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

static inline int hash_table_overload(const hash_table *ht)
{
    assert(ht != NULL);
    return ht->capacity == 0 || (double)ht->size > (double)ht->capacity * ht->load_factor;
}

static inline unsigned long hash_table_get_address(const hash_table *ht, const hash_table_key_type *key_ptr)
{
    assert(ht != NULL);
    assert(key_ptr != NULL);
    return *key_ptr & ht->mask;
}

static inline unsigned long linear_probing(const hash_table *ht, unsigned long addr)
{
    assert(ht != NULL);
    return (addr + 1) & ht->mask;
}

static void hash_table_rehash(hash_table *ht)
{
    unsigned long i;
    unsigned long addr;
    unsigned long new_capacity;
    hash_table_data_type *new_data = NULL;
    hash_table_bool_type *new_occupation = NULL;
    assert(ht != NULL);
    if (ht->capacity) {
        new_capacity = ht->capacity << 1;
        new_data = (hash_table_data_type *)malloc(new_capacity * sizeof(hash_table_data_type));
        assert(new_data != NULL);
        new_occupation = (hash_table_bool_type *)calloc(new_capacity, sizeof(hash_table_bool_type));
        assert(new_occupation != NULL);
        ht->mask = new_capacity - 1;
        for (i = 0; i < ht->capacity; ++i) {
            if (ht->occupation[i] && !ht->remove_flag[i]) {
                addr = hash_table_get_address(ht, &ht->data[i].key);
                while (new_occupation[addr])
                    addr = linear_probing(ht, addr);
                hash_table_data_copy(&new_data[addr], &ht->data[i]);
                new_occupation[addr] = 1;
            }
        }
        free(ht->data);
        ht->data = new_data;
        free(ht->occupation);
        ht->occupation = new_occupation;
        ht->remove_flag = (hash_table_bool_type *)realloc(ht->remove_flag, new_capacity * sizeof(hash_table_bool_type));
        assert(ht->remove_flag != NULL);
        memset(ht->remove_flag, 0, new_capacity * sizeof(hash_table_bool_type));
        ht->capacity = new_capacity;
    } else {
        ht->capacity = 1;
        while ((double)ht->capacity * ht->load_factor < 1)
            ht->capacity <<= 1;
        ht->mask = ht->capacity - 1;
        ht->data = (hash_table_data_type *)malloc(ht->capacity * sizeof(hash_table_data_type));
        assert(ht->data != NULL);
        ht->occupation = (hash_table_bool_type *)calloc(ht->capacity, sizeof(hash_table_bool_type));
        assert(ht->occupation != NULL);
        ht->remove_flag = (hash_table_bool_type *)calloc(ht->capacity, sizeof(hash_table_bool_type));
        assert(ht->remove_flag != NULL);
    }
}

hash_table_val_type *hash_table_find(hash_table *ht, const hash_table_key_type *key_ptr)
{
    unsigned long addr;
    assert(ht != NULL);
    assert(key_ptr != NULL);
    addr = hash_table_get_address(ht, key_ptr);
    while (ht->occupation[addr]) {
        if (!ht->remove_flag[addr] && hash_table_key_compare(&ht->data[addr].key, key_ptr) == 0)
            return &ht->data[addr].val;
        addr = linear_probing(ht, addr);
    }
    return NULL;
}

void hash_table_insert(hash_table *ht, const hash_table_data_type *data_ptr)
{
    unsigned long addr;
    assert(ht != NULL);
    assert(data_ptr != NULL);
    if (hash_table_overload(ht))
        hash_table_rehash(ht);
    addr = hash_table_get_address(ht, &data_ptr->key);
    while (ht->occupation[addr]) {
        if (ht->remove_flag[addr]) {
            ht->remove_flag[addr] = 0;
            break;
        }
        if (hash_table_key_compare(&ht->data[addr].key, &data_ptr->key) == 0) {
            hash_table_val_copy(&ht->data[addr].val, &data_ptr->val);
            return;
        }
        addr = linear_probing(ht, addr);
    }
    hash_table_data_copy(&ht->data[addr], data_ptr);
    ht->occupation[addr] = 1;
    ++ht->size;
}

void hash_table_delete(hash_table *ht, const hash_table_key_type *key_ptr)
{
    unsigned long addr;
    assert(ht != NULL);
    assert(key_ptr != NULL);
    addr = hash_table_get_address(ht, key_ptr);
    while (ht->occupation[addr]) {
        if (!ht->remove_flag[addr] && hash_table_key_compare(&ht->data[addr].key, key_ptr) == 0) {
            ht->remove_flag[addr] = 1;
            --ht->size;
            break;
        }
        addr = linear_probing(ht, addr);
    }
}

inline void hash_table_clear(hash_table *ht)
{
    assert(ht != NULL);
    memset(ht->occupation, 0, ht->capacity * sizeof(hash_table_bool_type));
    memset(ht->remove_flag, 0, ht->capacity * sizeof(hash_table_bool_type));
    ht->size = 0;
}

inline void hash_table_destroy(hash_table *ht)
{
    assert(ht != NULL);
    free(ht->data);
    free(ht->occupation);
    free(ht->remove_flag);
    ht->size = ht->capacity = 0;
}
