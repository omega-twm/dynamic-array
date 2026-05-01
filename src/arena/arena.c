#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "arena.h"

#define allocate_array(arena, type, count) \
  (type *)arena_allocate(arena, sizeof(type) * count)

#define allocate_struct(arena, type) \
  allocate_array(arena, type, 1)

#define allocate_struct_zero(arena, type ) \
  (type *)arena_allocate_zero(arena, sizeof(type))

struct arena {
  size_t capacity;
  size_t offset;
  u8 *memory;
};

arena_t arena_make(size_t capacity) {
  arena_t a;
  a.memory = malloc(capacity);
  a.capacity = capacity;
  a.offset = 0;
  return a;
}

void *arena_allocate_aligned(arena_t *a, size_t size, size_t align)
{
  size_t aligned_offset = (a->offset + (align - 1)) & ~(align - 1);
  assert(aligned_offset + size <= a->capacity && "Arena out of memory");

  void *ptr = a->memory + aligned_offset;
  a->offset = size + aligned_offset;

  return ptr;
}

void *arena_allocate_zero(arena_t *a, size_t size)
{
  void *ptr = arena_allocate(a, size);
  memset(ptr, 0, size);
  return ptr;
}

void *arena_allocate(arena_t *a, size_t size) {
  return arena_allocate_aligned(a, size, 16);
}

void arena_reset(arena_t *a) {
  a->offset = 0;
}

void arena_destroy(arena_t *a) {
  free(a->memory);
  a->memory = NULL;
  a->capacity = 0;
  a->offset = 0;
}

size_t arena_used(arena_t *a) { return a->offset; }

size_t arena_remaining(arena_t *a) { return a->capacity - a->offset; }

struct arena_save {
  arena_t *arena;
  size_t offset;
};

arena_save_t arena_save(arena_t *a)
{
  return (arena_save_t){ a, a->offset };
}

void arena_restore(arena_save_t save)
{
  save.arena->offset = save.offset;
}
