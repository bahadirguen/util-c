#ifndef _BG_ARENA_C
#define _BG_ARENA_C

#include <stddef.h>
#include <stdlib.h>

enum ArenaTypeEnum { ARENA_TYPE_ENUM_STATIC, ARENA_TYPE_ENUM_FIXED_BUMP };

typedef struct I_Arena_Allocator {
  size_t size;
  size_t idx;
  enum ArenaTypeEnum type_enum;
  void* buf;

  void* (*alloc)(struct I_Arena_Allocator*, const size_t);
  void (*free_arena)(struct I_Arena_Allocator*);
} I_Arena_Allocator;

I_Arena_Allocator* create_static_arena(const size_t size);

/*
 * INTERNAL
 */
void* alloc_static_arena(I_Arena_Allocator* arena, const size_t size) {
  // TODO : > or >= ?
  if ((arena->idx + size) > arena->size) {
    return NULL;
  }
  void* buf = (size_t*)arena->buf + size;
  arena->idx += size;
  return buf;
}

void free_static_arena(I_Arena_Allocator* arena) { free(arena); }

I_Arena_Allocator* create_static_arena(const size_t size) {
  const size_t struct_size = sizeof(I_Arena_Allocator);
  const size_t total_size = struct_size + size;
  void* buf = malloc(total_size);
  if (buf == NULL) {
    return NULL;
  }
  I_Arena_Allocator* arena = (I_Arena_Allocator*)buf;
  arena->buf = (size_t*)buf + struct_size;
  arena->size = size;
  arena->idx = 0;
  arena->type_enum = ARENA_TYPE_ENUM_STATIC;
  arena->alloc = alloc_static_arena;
  arena->free_arena = free_static_arena;
  return arena;
}

#endif  // _BG_ARENA_C
