#include <stdlib.h>
#include <string.h>

#include "dynamic-array.h"
#include "arena/arena.h"

struct dynamic_array {
  u8 *data; // byte pointer so we can do pointer arhitmatic to find the adress
            // to any element
  size_t elem_size;
  size_t length;
  size_t capacity;
  arena_t *arena;
};

dynamic_array_t *arr_create(arena_t *arena, size_t elem_size, size_t initial_capacity) {
  dynamic_array_t *arr = allocate_struct(arena, dynamic_array_t);

  arr->elem_size = elem_size;
  arr->length = 0;
  arr->capacity = initial_capacity;
  arr->data = arena_allocate(arena, elem_size * initial_capacity);
  arr->arena = arena;

  return arr;
}

void arr_destroy(dynamic_array_t *arr);

void arr_push(dynamic_array_t *arr, void *item) {

  // if there is no more room in the array, the capacity must double
  // check if there is room in arena?
  if (arr->capacity <= arr->length) {
    // calculate new capacity (double the old one)
    size_t new_capacity = arr->capacity * 2;

    // allocate new buffer from the arena for the elements
    u8 *new_data = arena_allocate(arr->arena, arr->elem_size * new_capacity);

    // copy existing elements over with memcpy
    // memcpy(buffer, arr, buffer->capacity);
    memcpy(new_data, arr->data, arr->elem_size * arr->length);

    arr->data = new_data;
    arr->capacity = new_capacity;
  }
  
  // is it fifo? then we add the element to the end of the array
  // find end of array; how do we do this? arr.elem_size * arr.length;
  u8 *slot = arr->data + (arr->elem_size * arr->length);
  memcpy(slot, item, arr->elem_size);

  arr->length += 1;
}

void *arr_pop(dynamic_array_t *arr);

/* return a ponter to the element at a given idx */
// what should happen if the caller passes an out of bounds index?
void *arr_get(dynamic_array_t *arr, size_t idx) {

  assert(idx < arr->length && "arr_get: index out of bounds");
  u8 *elem = arr->data + (arr->elem_size * idx);

  return elem;
}

size_t arr_getlength(dynamic_array_t *arr);

