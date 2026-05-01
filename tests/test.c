#define ARENA_IMPLEMENTATION
#define DYNAMIC_ARRAY_IMPLEMENTATION
#include <assert.h>
#include <stdio.h>
#include "common/common.h"
#include "arena/arena.h"
#include "dynamic_array/dynamic_array.h"

int main(void) {
    arena_t arena = arena_make(1024);
    dynamic_array_t *arr = dynarr_create(&arena, sizeof(i32), 2);

    i32 value = 42;
    dynarr_push(arr, &value);
    assert(dynarr_getlength(arr) == 1);
    assert(*(i32 *)dynarr_get(arr, 0) == 42);

    dynarr_pop(arr);
    assert(dynarr_getlength(arr) == 0);

    arena_destroy(&arena);
    printf("Basic dynamic array test passed.\n");
    return 0;
}
