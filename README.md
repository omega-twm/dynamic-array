# my-arena-libs

A small arena-backed collection library with a dynamic array example.

## Structure

- `arena/` — single-header arena allocator library
- `dynamic_array/` — single-header dynamic array library
- `common/` — shared type aliases and helper definitions
- `examples/` — example programs, including `array_example.c`
- `tests/` — simple test harness

## Build

Run the example with:

```sh
./build.sh
```

## Example

`examples/array_example.c` demonstrates `arr_create`, `arr_push`, `arr_getlength`, and `arr_pop`.
