#define ARENA_IMPLEMENTATION
#include "arena/arena.h"

#define DYNAMIC_ARRAY_IMPLEMENTATION
#include "dynamic_array/dynamic_array.h"

#include <stdio.h>
#include <stdlib.h>

void print_int(void *item) {
    printf("%d\n", *(int *)item);
}

int int_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return  1;
    return 0;
}

int main(void) {
    arena_t arena = arena_make(1024 * 1024);
    dynamic_array_t *arr = dynarr_create(&arena, sizeof(int), 4);

    printf("Enter numbers (Ctrl-D to stop):\n");
    int val;
    while (scanf("%d", &val) == 1) {
        dynarr_push(arr, &val);
    }

    printf("\nUnsorted (%zu items):\n", dynarr_getlength(arr));
    dynarr_print(arr, print_int);

    dynarr_sort(arr, int_cmp);

    printf("\nSorted:\n");
    dynarr_print(arr, print_int);

    arena_destroy(&arena);
    return EXIT_SUCCESS;
}