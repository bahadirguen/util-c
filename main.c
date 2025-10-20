#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.c"

char* alloc_char_array(I_Arena_Allocator* arena, const unsigned int num_char) {
  const size_t alloc_size = num_char * sizeof(char);
  void* buf = arena->alloc(arena, alloc_size);
  return (char*)buf;
}

int demo2(int argc, char** argv) {
  printf("Program %s: called with %d arguments\n", argv[0], argc);

  // const size_t arena_size = (1 << 10);
  // I_Arena_Allocator* arena = create_static_arena(arena_size);
  const size_t arena_size = 1024;
  I_Arena_Allocator* arena = create_fixed_bump_arena(arena_size);
  printf("Arena buf: %p, idx: %zu\n", arena->buf, arena->idx);

  int exit_code = EXIT_SUCCESS;
  const unsigned int num_chars_to_alloc = 500;
  for (size_t i = 0; i < 10; i++) {
    char* arr = alloc_char_array(arena, num_chars_to_alloc);
    if (arr == NULL) {
      fprintf(stderr, "Allocation attempt %zu failed, exiting\n", i + 1);
      exit_code = EXIT_FAILURE;
      goto label_exit;
    }
    printf("Arena buf: %p, idx: %zu\n", arena->buf, arena->idx);
  }
label_exit:
  arena->free_arena(arena);
  return exit_code;
}

int main(int argc, char** argv) { return demo2(argc, argv); }
