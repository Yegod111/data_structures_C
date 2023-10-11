/**
 * @File name: dynamic_array.c
 * @Description: Dynamic array implentations in C
 * @Date: 2022-2-22
*/

#include "dynamic_array.h"
#include <assert.h>
#include <stdlib.h>

#ifndef DEFAULT_DYNAMIC_ARRAY_CAPACITY
#define DEFAULT_DYNAMIC_ARRAY_CAPACITY 2
#endif // DYNAMIC_ARRAY_INIT_CAPACITY

// Initialize dynamic array with initial capacity
void dynamic_array_init(dynamic_array *arr, unsigned long init_capacity)
{
    assert(arr != NULL);
    if (init_capacity) {
	    arr->data = (dynamic_array_data_type *)calloc(init_capacity, sizeof(dynamic_array));
	    assert(arr->data != NULL);
	} else {
		arr->data = NULL;
	}
    arr->size = 0;
    arr->capacity = init_capacity;
}

// Memory expansion
inline void dynamic_array_expand(dynamic_array *arr)
{
	assert(arr != NULL);
    if (arr->capacity)
        arr->capacity <<= 1;
    else
        arr->capacity = DEFAULT_DYNAMIC_ARRAY_CAPACITY;
    arr->data = (dynamic_array_data_type *)realloc(arr->data, arr->capacity * sizeof(dynamic_array_data_type));
    assert(arr->data != NULL);
}

// Return the size of the dynamic array
inline unsigned long dynamic_array_size(const dynamic_array *arr)
{
    assert(arr != NULL);
    return arr->size;
}

// Return the capacity of the dynamic array
inline unsigned long dynamic_array_capacity(const dynamic_array *arr)
{
    assert(arr != NULL);
    return arr->capacity;
}

// Is the dynamic array empty
inline int dynamic_array_empty(const dynamic_array *arr)
{
    assert(arr != NULL);
    return arr->size == 0;
}

// Is the dyanmic array full
inline int dynamic_array_full(const dynamic_array *arr)
{
    assert(arr != NULL);
    return arr->size == arr->capacity;
}

// Get the data whose index is idx
inline dynamic_array_data_type dynamic_array_get(const dynamic_array *arr, unsigned long idx)
{
    assert(arr != NULL && idx < arr->size);
    return arr->data[idx];
}

// Insert the entry at index idx
inline void dynamic_array_insert(dynamic_array *arr, unsigned long idx, dynamic_array_data_type entry)
{
	unsigned long i;
    assert(arr != NULL && idx <= arr->size);
    if (dynamic_array_full(arr))
        dynamic_array_expand(arr);
    for (i = arr->size; i > idx; --i)
    	arr->data[i] = arr->data[i - 1];
    arr->data[idx] = entry;
    ++arr->size;
}

// Delete the data whose index is idx
inline void dynamic_array_delete(dynamic_array *arr, unsigned long idx)
{
    unsigned long i;
    assert(arr != NULL && idx < arr->size);
    --arr->size;
    for (i = idx; i < arr->size; ++i)
        arr->data[i] = arr->data[i + 1];
}

// Assign the data whose index is idx to entry
inline void dynamic_array_assign(dynamic_array *arr, unsigned long idx, dynamic_array_data_type entry)
{
    assert(arr != NULL && idx < arr->size);
    arr->data[idx] = entry;
}

// Can the data whose value is entry be found 
inline int dynamic_array_find(const dynamic_array *arr, dynamic_array_data_type entry)
{
    unsigned long i;
    assert(arr != NULL);
    for (i = 0; i < arr->size; ++i)
        if (arr->data[i] == entry)
            return 1;
    return 0;
}

// Clear all the data in the dynamic array
inline void dynamic_array_clear(dynamic_array *arr)
{
    assert(arr != NULL);
    arr->size = 0;
}

// Destroy the dynamic array
inline void dynamic_array_destroy(dynamic_array *arr)
{
    assert(arr != NULL);
    if (arr->data != NULL) {
        free(arr->data);
        arr->data = NULL;
        arr->size = arr->capacity = 0;
    }
}

#define SWAP_DYNAMIC_ARRAY_DATA(data_a, data_b) \
    do {\
        dynamic_array_data_type tmp = data_a;\
        data_a = data_b;\
        data_b = tmp;\
    } while (0)

// Reverse the dyanmic array
inline void dynamic_array_reverse(dynamic_array *arr)
{
    unsigned long i;
    unsigned long len = arr->size >> 1;
    unsigned long end = arr->size - 1;
    assert(arr != NULL);
    for (i = 0; i < len; ++i)
        SWAP_DYNAMIC_ARRAY_DATA(arr->data[i], arr->data[end - i]);
}
