#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

/**
 * @file dynamic_array.h
 * @brief Public interface for a simple dynamic array allocator.
 *
 * The dynamic array stores elements of a fixed size and grows automatically
 * as new items are pushed. The array is backed by a memory arena provided by
 * the caller.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "arena/arena.h"

/** Opaque dynamic array handle. */
typedef struct dynamic_array dynamic_array_t;

typedef void (*dynamic_array_print_fn)(void *item);
typedef int (*dynamic_array_cmp_fn)(const void *a, const void *b);

/**
 * @brief Create a new dynamic array.
 *
 * @param arena Pointer to the arena used for allocation.
 * @param elem_size Size of each element in bytes.
 * @param initial_capacity Initial number of elements the array can hold.
 * @return Pointer to the newly created dynamic array, or aborts on allocation failure.
 */
dynamic_array_t *dynarr_create(arena_t *arena, size_t elem_size, size_t initial_capacity);

/**
 * @brief Destroy a dynamic array.
 *
 * @param arr Pointer to the dynamic array to destroy.
 *
 * This function releases the array itself. The contents of the arena are
 * managed by the arena allocator, so individual elements are not freed here.
 */
void dynarr_destroy(dynamic_array_t *arr);

/**
 * @brief Append an item to the end of the array.
 *
 * @param arr Pointer to the dynamic array.
 * @param item Pointer to the item data to copy into the array.
 */
void dynarr_push(dynamic_array_t *arr, void *item);

/**
 * @brief Remove and return the last item from the array.
 *
 * @param arr Pointer to the dynamic array.
 * @return Pointer to the popped item data, or NULL if the array is empty.
 *
 * The returned pointer points into the array's internal storage and is valid
 * until the array is reallocated or destroyed.
 */
void *dynarr_pop(dynamic_array_t *arr);

/**
 * @brief Retrieve an item by index.
 *
 * @param arr Pointer to the dynamic array.
 * @param idx Zero-based index of the item.
 * @return Pointer to the item at the given index.
 *
 * The caller must ensure idx is less than the current array length.
 */
void *dynarr_get(dynamic_array_t *arr, size_t idx);

/**
 * @brief Get the current number of elements in the array.
 *
 * @param arr Pointer to the dynamic array.
 * @return Number of stored elements.
 */
size_t dynarr_getlength(dynamic_array_t *arr);

void dynarr_print(dynamic_array_t *arr, dynamic_array_print_fn print);

void dynarr_sort(dynamic_array_t *arr, dynamic_array_cmp_fn cmp);

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION

#include <assert.h>
#include <string.h>
#include <stdio.h>

struct dynamic_array {
    uint8_t *data;
    size_t elem_size;
    size_t length;
    size_t capacity;
    arena_t *arena;
};

dynamic_array_t *dynarr_create(arena_t *arena, size_t elem_size, size_t initial_capacity) {
    dynamic_array_t *arr = arena_alloc_struct(arena, dynamic_array_t);

    arr->elem_size = elem_size;
    arr->length = 0;
    arr->capacity = initial_capacity;
    arr->data = arena_allocate(arena, elem_size * initial_capacity);
    arr->arena = arena;

    return arr;
}

void dynarr_destroy(dynamic_array_t *arr) {
    arr->data = NULL;
    arr->length = 0;
    arr->capacity = 0;
}

void dynarr_push(dynamic_array_t *arr, void *item) {
    if (arr->capacity <= arr->length) {
        size_t new_capacity = arr->capacity * 2;
        uint8_t *new_data = arena_allocate(arr->arena, arr->elem_size * new_capacity);
        memcpy(new_data, arr->data, arr->elem_size * arr->length);
        arr->data = new_data;
        arr->capacity = new_capacity;
    }

    uint8_t *slot = arr->data + (arr->elem_size * arr->length);
    memcpy(slot, item, arr->elem_size);
    arr->length += 1;
}

void *dynarr_pop(dynamic_array_t *arr) {
    if (arr->length == 0) {
        return NULL;
    }
    uint8_t *elem = arr->data + (arr->elem_size * (arr->length - 1));
    arr->length -= 1;
    return elem;
}

void *dynarr_get(dynamic_array_t *arr, size_t idx) {
    assert(idx < arr->length && "dynarr_get: index out of bounds");
    return arr->data + (arr->elem_size * idx);
}

size_t dynarr_getlength(dynamic_array_t *arr) {
    return arr->length;
}

void dynarr_print(dynamic_array_t *arr, dynamic_array_print_fn print) {
    for (size_t i = 0; i < arr->length; i++) {
        print(dynarr_get(arr, i));
    }
}

void dynarr_sort(dynamic_array_t *arr, dynamic_array_cmp_fn cmp) {
    qsort(arr->data, arr->length, arr->elem_size, cmp);
}

#endif // DYNAMIC_ARRAY_IMPLEMENTATION



#endif // DYNAMIC_ARRAY_H