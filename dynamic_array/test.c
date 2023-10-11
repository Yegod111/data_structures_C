#include "dynamic_array.h"
#include <stdio.h>

void output(const dynamic_array *);

inline void output(const dynamic_array *arr)
{
    unsigned long i;
    for (i = 0; i < dynamic_array_size(arr); ++i) {
        if (i)
            printf(" ");
        printf("%d", dynamic_array_get(arr, i));
    }
    printf("\n");
}

int main(void)
{
    int i;
    dynamic_array arr;
    dynamic_array_init(&arr, 0);
    for (i = 0; i < 100; ++i)
        dynamic_array_insert(&arr, i, i);
    output(&arr);
    for (i = 0; i < dynamic_array_size(&arr); i += 2)
        dynamic_array_delete(&arr, i);
    output(&arr);
    dynamic_array_reverse(&arr);
    output(&arr);
    dynamic_array_destroy(&arr);
    return 0;
}
