#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

typedef int hash_table_key_type;
typedef int hash_table_val_type;
typedef struct HashTableDataNode
{
    hash_table_key_type key;
    hash_table_val_type val;
} hash_table_data_type;
typedef struct HashTableListNode
{
    hash_table_data_type data;
    struct HashTableListNode *next;
} hash_table_list_node;
typedef struct HashTable
{
    hash_table_list_node **hash_list;
    unsigned long size;
    unsigned long capacity;
    unsigned long mask;
    double load_factor;
} hash_table;

void hash_table_init(hash_table *, double);
unsigned long hash_table_size(const hash_table *);
unsigned long hash_table_capacity(const hash_table *);
int hash_table_empty(const hash_table *);
hash_table_val_type *hash_table_find(hash_table *, const hash_table_key_type *);
void hash_table_insert(hash_table *, const hash_table_data_type *);
void hash_table_delete(hash_table *, const hash_table_key_type *);
void hash_table_clear(hash_table *);
void hash_table_destroy(hash_table *);

#endif // __HASH_TABLE_H__