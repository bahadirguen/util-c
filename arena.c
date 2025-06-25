#ifndef _BG_ARENA_C
#define _BG_ARENA_C

#include <stddef.h>
#include <stdlib.h>

typedef struct SimpleArena {
  void* buf;
  size_t size;
  size_t idx;
} SimpleArena;

SimpleArena* initArena(const size_t size) {
  const size_t struct_size = sizeof(SimpleArena);
  const size_t total_size = struct_size + size;
  void* buf = malloc(total_size);
  if (buf == NULL) {
    return NULL;
  }
  SimpleArena* arena = (SimpleArena*)buf;
  arena->buf = (size_t*)buf + struct_size;
  arena->size = size;
  arena->idx = 0;
  return arena;
}

void freeArena(SimpleArena* arena) { free(arena); }

void* allocArena(SimpleArena* arena, const size_t size) {
  // TODO : > or >= ?
  if ((arena->idx + size) > arena->size) {
    return NULL;
  }
  void* buf = (size_t*)arena->buf + size;
  arena->idx += size;
  return buf;
}

#endif  // _BG_ARENA_C
