#ifndef _BG_ARENA_C
#define _BG_ARENA_C

#include <stdint.h>
#include <stdlib.h>

typedef struct SimpleArena {
  void* buf;
  uint32_t size;
  uint32_t idx;
} SimpleArena;

SimpleArena* initArena(uint32_t size) {
  const uint32_t struct_size = sizeof(SimpleArena);
  const uint32_t total_size = struct_size + size;
  void* buf = malloc(total_size);
  if (buf == NULL) {
    return NULL;
  }
  SimpleArena* arena = (SimpleArena*)buf;
  arena->buf = (uint32_t*)buf + struct_size;
  arena->size = size;
  arena->idx = 0;
  return arena;
}

void freeArena(SimpleArena* arena) { free(arena); }

void* allocArena(SimpleArena* arena, uint32_t size) {
  // TODO : > or >= ?
  if ((arena->idx + size) > arena->size) {
    return NULL;
  }
  void* buf = (uint32_t*)arena->buf + size;
  arena->idx += size;
  return buf;
}

#endif  // _BG_ARENA_C
