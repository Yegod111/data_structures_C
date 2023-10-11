#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN (1 << 24)
#define OFFSET 5211314

int main()
{
    int i;
    int key;
    int cnt = 0;
    clock_t begin, end;
    hash_table *ht = (hash_table *)malloc(sizeof(hash_table));
    hash_table_init(ht, 0.5);

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        key = (rand() << 8) - OFFSET + (i & 1 ? i : -i);
        hash_table_insert(ht, &(hash_table_data_type){key, i});
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%lu %lu\n", hash_table_size(ht), hash_table_capacity(ht));
    
    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < (MAXN >> 2); ++i) {
        key = (rand() << 8) - OFFSET + (i & 1 ? i : -i);
        hash_table_delete(ht, &key);
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%lu %lu\n", hash_table_size(ht), hash_table_capacity(ht));

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < (MAXN >> 1); ++i) {
        key = (rand() << 8) - OFFSET + (i & 1 ? i : -i);
        if (hash_table_find(ht, &key) != NULL)
            ++cnt;
    }
    end = clock();
    printf("%lldms\n", end - begin);
    printf("%d\n", cnt);

    hash_table_destroy(ht);
    free(ht);
    return 0;
}