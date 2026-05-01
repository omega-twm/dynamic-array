#include <stdlib.h>

#include "common/common.h"
#include "arena/arena.c"

typedef struct dynamic_array dynamic_array_t;
 
dynamic_array_t *arr_create(arena_t *arena, size_t elem_size, size_t initial_capacity);
void arr_destroy(dynamic_array_t *arr);
void arr_push(dynamic_array_t *arr, void *item);
void *arr_pop(dynamic_array_t *arr);
void *arr_get(dynamic_array_t *arr, size_t idx);
size_t arr_getlength(dynamic_array_t *arr);

