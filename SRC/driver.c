#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "llist.h"

#define TESTS 20

void test_func(bool (*in)(llist_t *, void *), bool (*out)(llist_t *, void **));

bool has_run = false;

int main()
{
    srand(time(NULL));
    printf("Testing Queue (First In First Out)\n");
    test_func(llist_enqueue, llist_dequeue);

    printf("\nTesting Stack (Last In First Out)\n");
    test_func(llist_push, llist_pop);
}

void test_func(bool (*in)(llist_t *, void *), bool (*out)(llist_t *, void **))
{
    llist_t *list = llist_create(free);
    if (!list) {
        fprintf(stderr, "Failed making linked-list\n");
        return;
    }

    int offset = 65;
    if (has_run) {
        offset = 97;
    }

    for (int i = 0; i < TESTS; ++i) {

        // Pseudo-random choice to add/remove item
        //
        int action = rand();
        if (action % 2) {
            char *letter = malloc(sizeof(*letter));
            if (!letter) {
                fprintf(stderr, "Failed alloc for letter\n");
            }
            *letter = rand() % 26 + offset;
            printf("  +++ %c\n", *letter);
            in(list, letter);
        } else {
            void *data = NULL;
            if (out(list, &data)) {
                printf("   --- %c\n", *((char*)data));
                free(data);
            } 
        }
    }

    if (!llist_is_empty(list)) {
        void *data = NULL;
        while (out(list, &data)) {
            printf("   --- %c\n", *((char*)data));
            free(data);
        }
    }
    has_run = true;
    llist_delete(&list);
}