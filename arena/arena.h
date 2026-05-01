#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

#define arena_alloc_array(arena, type, count)                                     \
  (type *)arena_allocate(arena, sizeof(type) * (count))

#define arena_alloc_struct(arena, type) arena_alloc_array(arena, type, 1)

#define arena_alloc_struct_zero(arena, type)                                      \
  (type *)arena_allocate_zero(arena, sizeof(type))

/*!
 * Type of arena. 'arena_t' is an alias for 'struct arena'
 */
typedef struct arena arena_t;

/**
 * Arena implementation details made visible in the public header so callers can
 * allocate and initialize arena_t values directly.
 */
struct arena {
    size_t capacity;
    size_t offset;
    uint8_t *memory;
};

/*! 
 * Type of arena save. 'arena_save_t' is an alias for 'struct arena_save'
 *
 * This lets us do temporary work, like building a temprary array mid function
 * without needing a seperate scratch arena
 */
typedef struct arena_save arena_save_t;

struct arena_save {
    arena_t *arena;
    size_t offset;
};

arena_t arena_make(size_t capacity);

void *arena_allocate_aligned(arena_t *a, size_t size, size_t align);

void *arena_allocate_zero(arena_t *a, size_t size);

void *arena_allocate(arena_t *a, size_t size);

void arena_reset(arena_t *a);

void arena_destroy(arena_t *a);

/*!
 * Returns area used in given arena
 */
size_t arena_used(arena_t *a);

/*!
 * Returns remaining area used in given arena
 */
size_t arena_remaining(arena_t *a);

/*!
 * @brief Saves the arena state
 * @param a: An arena that has already been created
 * @returns The saved state of given arena
 */
arena_save_t arena_save(arena_t *a);

/*!
 * Restores arena to previous state
 *
 * usage:
 *    'arena_save_t save = arena_save(a);
 *     // do temorary work...
 *     arena_restore(save); // rolls back as if the allocations never happened
 *    '
 * @param save: arena save state
 */
void arena_restore(arena_save_t save);

#ifdef ARENA_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <string.h>

arena_t arena_make(size_t capacity) {
    arena_t a;
    a.memory = malloc(capacity);
    a.capacity = capacity;
    a.offset = 0;
    return a;
}

void *arena_allocate_aligned(arena_t *a, size_t size, size_t align) {
    size_t aligned_offset = (a->offset + (align - 1)) & ~(align - 1);
    assert(aligned_offset + size <= a->capacity && "Arena out of memory");

    void *ptr = a->memory + aligned_offset;
    a->offset = size + aligned_offset;
    return ptr;
}

void *arena_allocate_zero(arena_t *a, size_t size) {
    void *ptr = arena_allocate(a, size);
    memset(ptr, 0, size);
    return ptr;
}

void *arena_allocate(arena_t *a, size_t size) {
    return arena_allocate_aligned(a, size, 16);
}

void arena_reset(arena_t *a) { a->offset = 0; }

void arena_destroy(arena_t *a) {
    free(a->memory);
    a->memory = NULL;
    a->capacity = 0;
    a->offset = 0;
}

size_t arena_used(arena_t *a) { return a->offset; }

size_t arena_remaining(arena_t *a) { return a->capacity - a->offset; }

arena_save_t arena_save(arena_t *a) { return (arena_save_t){a, a->offset}; }

void arena_restore(arena_save_t save) { save.arena->offset = save.offset; }

#endif // ARENA_IMPLEMENTATION

#endif // ARENA_H