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

    const size_t last_idx = alloc_size - 1;
    arr->head[last_idx] = 2 * (i + 1);
    printf("arr[%zu]: %d\n", last_idx, arr->head[last_idx]);
  }
  freeArena(arena);
  return EXIT_SUCCESS;
}
