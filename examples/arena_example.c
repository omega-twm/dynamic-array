#include <stdio.h>
#include "common/common.h"
#include "arena/arena.h"

int main(void) {
    arena_t arena = arena_make(1024 * 1024);

    printf("Arena capacity: %zu bytes\n", arena.capacity);
    printf("Arena remaining: %zu bytes\n", arena_remaining(&arena));

    arena_destroy(&arena);
    return 0;
}
