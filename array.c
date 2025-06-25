#ifndef _BG_ARRAY_C
#define _BG_ARRAY_C

#include <stddef.h>
#include <stdint.h>

#include "arena.c"

typedef struct U32Array {
  uint32_t* head;
  size_t size;
} U32Array;

U32Array* allocU32Array(SimpleArena* arena, const size_t size) {
  const size_t struct_size = sizeof(U32Array);
  const size_t total_size = struct_size + size;
  void* buf = allocArena(arena, total_size);
  if (buf == NULL) {
    return NULL;
  }
  U32Array* arr = (U32Array*)buf;
  arr->head = (uint32_t*)((size_t*)(buf) + struct_size);
  arr->size = size;
  return arr;
}

#endif  // _BG_ARRAY_C
