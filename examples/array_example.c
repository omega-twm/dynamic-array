#define ARENA_IMPLEMENTATION
#define ARENA_IMPLEMENTATION
#define DYNAMIC_ARRAY_IMPLEMENTATION
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/common.h"
#include "arena/arena.h"
#include "dynamic_array/dynamic_array.h"

void print_int(void *item) {
    printf("%d\n", *(int *)item);
}

i32 main(void) 
{
  arena_t arena = arena_make(1024 * 1024);
  
  dynamic_array_t *arr = dynarr_create(&arena, sizeof(i32), 3);  
  dynarr_print(arr, print_int);

  for (size_t i = 0; i < 10; i++) {
    i32 val = i; 
    dynarr_push(arr, &val);
  }
  printf("printing after push:\n");
  dynarr_print(arr, print_int);

  printf("length: %lu\n", dynarr_getlength(arr));
  dynarr_pop(arr);
  printf("length: %lu\n", dynarr_getlength(arr));
  
  dynarr_destroy(arr);
  printf("print after dynarr_destroy\n");
  dynarr_print(arr, print_int);

  arena_destroy(&arena);
  return EXIT_SUCCESS;

}
