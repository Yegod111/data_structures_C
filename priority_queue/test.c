#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN (1 << 22 | 521)
#define OFFSET 5211314

int main(void)
{
    unsigned long i;
    int key;
    int cnt = 0;
    clock_t begin, end;
    priority_queue_data_type data;
    priority_queue *que = (priority_queue *)malloc(sizeof(priority_queue));
    priority_queue_init(que);

    srand((unsigned int)time(NULL));
    begin = clock();
    for (i = 0; i < MAXN; ++i) {
        key = (rand() << 8) - OFFSET + i;
        priority_queue_push(que, &(priority_queue_data_type){key, key});
    }
    end = clock();
    printf("\n%lldms\n", end - begin);
    printf("%u %u\n", priority_queue_size(que), priority_queue_capacity(que));

    begin = clock();
    while (!priority_queue_empty(que)) {
        if (priority_queue_top(que).val > 0)
            ++cnt;
        priority_queue_pop(que);
    }
    end = clock();
    printf("\n%lldms\n", end - begin);
    printf("%d\n", cnt);

    priority_queue_destroy(que);
    free(que);
    return 0;
}