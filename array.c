#ifndef _BG_ARRAY_C
#define _BG_ARRAY_C

#include <stddef.h>
#include <stdint.h>

#include "arena.c"

typedef struct U32Array {
  uint32_t* head;
  size_t size;
} U32Array;

/*
 * 0: no access error,
 * 1: out-of-bounds access,
 */
enum ArrayAccessError { NO_ARRAY_ACCESS_ERROR = 0, OUT_OF_BOUNDS_ACCESS };

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

enum ArrayAccessError u32ArrayGet(U32Array* arr, const size_t idx,
                                  uint32_t* out) {
  if (idx >= arr->size) {
    return OUT_OF_BOUNDS_ACCESS;
  }
  *out = arr->head[idx];
  return NO_ARRAY_ACCESS_ERROR;
}

enum ArrayAccessError u32ArraySet(U32Array* arr, const size_t idx,
                                  const uint32_t val) {
  if (idx >= arr->size) {
    return OUT_OF_BOUNDS_ACCESS;
  }
  arr->head[idx] = val;
  return NO_ARRAY_ACCESS_ERROR;
}

#endif  // _BG_ARRAY_C
