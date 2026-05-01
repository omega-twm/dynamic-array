/**
 * @file dynamic-array.h
 * @brief Public interface for a simple dynamic array allocator.
 *
 * The dynamic array stores elements of a fixed size and grows automatically
 * as new items are pushed. The array is backed by a memory arena provided by
 * the caller.
 */

#include <stdlib.h>

#include "common/common.h"
#include "arena/arena.c"

/** Opaque dynamic array handle. */
typedef struct dynamic_array dynamic_array_t;
 
/**
 * @brief Create a new dynamic array.
 *
 * @param arena Pointer to the arena used for allocation.
 * @param elem_size Size of each element in bytes.
 * @param initial_capacity Initial number of elements the array can hold.
 * @return Pointer to the newly created dynamic array, or NULL on allocation failure.
 */
dynamic_array_t *arr_create(arena_t *arena, size_t elem_size, size_t initial_capacity);

/**
 * @brief Destroy a dynamic array.
 *
 * @param arr Pointer to the dynamic array to destroy.
 *
 * This function releases the array itself. The contents of the arena are
 * managed by the arena allocator, so individual elements are not freed here.
 */
void arr_destroy(dynamic_array_t *arr);

/**
 * @brief Append an item to the end of the array.
 *
 * @param arr Pointer to the dynamic array.
 * @param item Pointer to the item data to copy into the array.
 */
void arr_push(dynamic_array_t *arr, void *item);

/**
 * @brief Remove and return the last item from the array.
 *
 * @param arr Pointer to the dynamic array.
 * @return Pointer to the popped item data, or NULL if the array is empty.
 *
 * The returned pointer points into the array's internal storage and is valid
 * until the array is reallocated or destroyed.
 */
void *arr_pop(dynamic_array_t *arr);

/**
 * @brief Retrieve an item by index.
 *
 * @param arr Pointer to the dynamic array.
 * @param idx Zero-based index of the item.
 * @return Pointer to the item at the given index.
 *
 * The caller must ensure idx is less than the current array length.
 */
void *arr_get(dynamic_array_t *arr, size_t idx);

/**
 * @brief Get the current number of elements in the array.
 *
 * @param arr Pointer to the dynamic array.
 * @return Number of stored elements.
 */
size_t arr_getlength(dynamic_array_t *arr);

