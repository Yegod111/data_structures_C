/**
 * @File name: dynamic_array.h
 * @Description: Header for dynamic array in C
 * @Date: 2023-2-22
*/

#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

typedef int dynamic_array_data_type;
typedef struct DynamicArray
{
    dynamic_array_data_type *data;
    unsigned long size;
    unsigned long capacity;
} dynamic_array;

void dynamic_array_init(dynamic_array *, unsigned long);
void dynamic_array_expand(dynamic_array *);
unsigned long dynamic_array_size(const dynamic_array *);
unsigned long dynamci_array_capacity(const dynamic_array *);
int dynamic_array_empty(const dynamic_array *);
int dynamic_array_full(const dynamic_array *);
dynamic_array_data_type dynamic_array_get(const dynamic_array *, unsigned long);
void dynamic_array_insert(dynamic_array *, unsigned long, dynamic_array_data_type);
void dynamic_array_delete(dynamic_array *, unsigned long);
void dynamic_array_assign(dynamic_array *, unsigned long, dynamic_array_data_type);
int dynamic_array_find(const dynamic_array *, dynamic_array_data_type);
void dynamic_array_reverse(dynamic_array *);
void dynamic_array_clear(dynamic_array *);
void dynamic_array_destroy(dynamic_array *);

#endif // __DYNAMIC_ARRAY_H__