#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.c"
#include "array.c"

int main(int argc, char** argv) {
  printf("Program %s: called with %d arguments\n", argv[0], argc);
  uint32_t arena_size = (1 << 10);
  SimpleArena* arena = initArena(arena_size);
  printf("Arena buf: %p\n", arena->buf);
  printf("Arena idx: %d\n", arena->idx);
  uint32_t alloc_size = (1 << 8);
  for (uint32_t i = 0; i < 5; i++) {
    // void* buf = allocArena(arena, alloc_size);
    U32Array* arr = allocU32Array(arena, alloc_size);
    // if (buf == NULL) {
    if (arr == NULL) {
      fprintf(stderr, "Allocation attempt %d failed, exiting\n", i + 1);
      break;
    }
    printf("Arena idx: %d\n", arena->idx);

    // uint32_t* arr = (uint32_t*)buf;
    // arr[alloc_size - 1] = 2 * (i + 1);
    // printf("arr[%d]: %d\n", alloc_size - 1, arr[alloc_size - 1]);
    arr->head[alloc_size - 1] = 2 * (i + 1);
    printf("arr[%d]: %d\n", alloc_size - 1, arr->head[alloc_size - 1]);
  }
  freeArena(arena);
  return EXIT_SUCCESS;
}
