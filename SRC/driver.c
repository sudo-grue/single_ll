#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "llist.h"

#define TESTS 10

void test_func(bool (*in)(llist_t *, void *), bool (*out)(llist_t *, void **));

int main()
{
    srand(time(NULL));
    printf("Testing Queue\n");
    test_func(llist_enqueue, llist_dequeue);

    printf("\nTesting Stack\n");
    test_func(llist_push, llist_pop);
}

void test_func(bool (*in)(llist_t *, void *), bool (*out)(llist_t *, void **))
{
    llist_t *list = llist_create(free);
    if (!list) {
        fprintf(stderr, "Failed making list\n");
        return;
    }

    for (int i = 0; i < TESTS; ++i) {
        int action = rand();
        if (action % 2) {
            int *num = malloc(sizeof(*num));
            *num = rand() % 26 + 65;
            printf("  +++ %c\n", *num);
            in(list, num);
        } else {
            void *data = NULL;
            if (out(list, &data)) {
                printf("  --- %c\n", *((char*)data));
                free(data);
            } 
        }
    }

    if (!llist_is_empty(list)) {
        printf("  Purging Remainder\n");
        void *data = NULL;
        while (out(list, &data)) {
            printf("  --- %c\n", *((char*)data));
            free(data);
        }
    }
    llist_delete(&list);
}