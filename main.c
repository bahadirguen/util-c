#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.c"
#include "array.c"

int main(int argc, char** argv) {
  printf("Program %s: called with %d arguments\n", argv[0], argc);

  const size_t arena_size = (1 << 10);
  SimpleArena* arena = initArena(arena_size);
  printf("Arena buf: %p\n", arena->buf);
  printf("Arena idx: %zu\n", arena->idx);
  const size_t alloc_size = (1 << 8);
  for (size_t i = 0; i < 5; i++) {
    U32Array* arr = allocU32Array(arena, alloc_size);
    if (arr == NULL) {
      fprintf(stderr, "Allocation attempt %zu failed, exiting\n", i + 1);
      break;
    }
    printf("Arena idx: %zu\n", arena->idx);

    // const size_t last_idx = arr->size - 1;
    const size_t last_idx = arr->size;
    int32_t rc = u32ArraySet(arr, last_idx, 2 * (i + 1));
#ifndef NDEBUG
    if (rc != NO_ARRAY_ACCESS_ERROR) {
      fprintf(stderr, "ArraySet error (rc: %d) with idx %zu, exiting\n", rc,
              last_idx);
      break;
    }
#endif  // NDEBUG
    uint32_t last_elem;
    rc = u32ArrayGet(arr, last_idx, &last_elem);
#ifndef NDEBUG
    if (rc != NO_ARRAY_ACCESS_ERROR) {
      fprintf(stderr, "ArrayGet error (rc: %d) with idx %zu, exiting\n", rc,
              last_idx);
      break;
    }
#endif  // NDEBUG
    printf("arr[%zu]: %d (rc: %d)\n", last_idx, last_elem, rc);
  }
  freeArena(arena);
  return EXIT_SUCCESS;
}
