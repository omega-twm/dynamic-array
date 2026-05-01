#ifndef COMMON_COMMON_H
#define COMMON_COMMON_H

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef i8 b8;
typedef i32 b32;

typedef void (*print_fn)(void *item);
typedef int (*cmp_fn)(const void *a, const void *b);

// negative if a<b 0 if equal, positive if a>b
static inline i32 intcmp(const void *a, const void *b) {
    i32 ia = *(const i32 *)a;
    i32 ib = *(const i32 *)b;
    if (ia < ib) return -1;
    if (ia > ib) return  1;
    return 0;
}

#endif // COMMON_COMMON_H
