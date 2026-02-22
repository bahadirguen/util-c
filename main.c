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

I_Arena_Allocator* create_arena_from_args(int argc, char** argv) {
  size_t arena_size = 1024;  // 1 << 10
  if (argc > 2) {
    char* arg_arena_size_end = NULL;
    size_t arg_arena_size = strtoul(argv[2], &arg_arena_size_end, 10);
    if (!*arg_arena_size_end) {
      arena_size = arg_arena_size;
    } else {
      printf(
          "Could not convert '%s' to unsigned integer, using arena size %zu\n",
          argv[2], arena_size);
    }
  }

  I_Arena_Allocator* arena = NULL;
  I_Arena_Allocator* (*create_default_arena)(const size_t) =
      create_static_arena;
  if (argc > 1) {
    char* arg_arena_type_end = NULL;
    int arg_arena_type = strtol(argv[1], &arg_arena_type_end, 10);
    if (!*arg_arena_type_end) {
      switch (arg_arena_type) {
        case ARENA_TYPE_ENUM_STATIC:
          arena = create_static_arena(arena_size);
          break;
        case ARENA_TYPE_ENUM_FIXED_BUMP:
          arena = create_fixed_bump_arena(arena_size);
          break;
        default:
          arena = create_default_arena(arena_size);
          printf("Arena type '%d' is not recognized, using arena type %d\n",
                 arg_arena_type, arena->type_enum);
      };
    } else {
      arena = create_default_arena(arena_size);
      printf(
          "Could not convert '%s' to unsigned integer, using arena type %d\n",
          argv[1], arena->type_enum);
    }
  } else {
    arena = create_default_arena(arena_size);
  }

  return arena;
}

int demo2(int argc, char** argv) {
  printf("Program %s: called with %d arguments\n", argv[0], argc);

  int exit_code = EXIT_SUCCESS;
  I_Arena_Allocator* arena = create_arena_from_args(argc, argv);
  if (!arena) {
    printf("Internal logic error: Could not initialize arena, aborting!\n");
    exit_code = EXIT_FAILURE;
    goto label_exit;
  }

  printf("Using arena type %d with size %zu\n", arena->type_enum, arena->size);
  printf("Arena buf: %p, idx: %zu\n", arena->buf, arena->idx);

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
