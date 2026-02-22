#ifndef _BG_ARENA_C
#define _BG_ARENA_C

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define FIXED_BUMP_ARENA_NUM_MAX_ALLOCATIONS 256

enum ArenaTypeEnum {
  ARENA_TYPE_ENUM_STATIC,
  ARENA_TYPE_ENUM_FIXED_BUMP,
  ARENA_TYPE_ENUM__NUM_ELEMENTS
};

typedef struct I_Arena_Allocator {
  size_t size;
  size_t idx;
  enum ArenaTypeEnum type_enum;
  void* buf;
  void* arena_ctx;

  void* (*alloc)(struct I_Arena_Allocator*, const size_t);
  void (*free_arena)(struct I_Arena_Allocator*);
} I_Arena_Allocator;

/*
 * One malloc, continuous memory layout: I_Arena_Allocator, arena_ctx, buf
 */
I_Arena_Allocator* create_static_arena(const size_t arena_size);

/*
 * One malloc for struct block and one malloc per data block (bump).
 * Allocates one data block on initialization.
 * I_Arena_Allocator->size always contains the value of data_block_size,
 * not the total capacity.
 * I_Arena_Allocator->idx contains the index of the current data block.
 * Struct block memory layout: I_Arena_Allocator, arena_ctx
 *
 * If an allocation is about to exceed the remaining capacity
 * of the current data block, then mallocs a new data block.
 * Does not allocate if more memory than the data block size is requested.
 *
 * Returned pointers will remain in the same memory address after creation.
 * Returned pointers will have continuous memory for the allocation size.
 */
I_Arena_Allocator* create_fixed_bump_arena(const size_t data_block_size);

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

I_Arena_Allocator* create_static_arena(const size_t arena_size) {
  const size_t struct_size = sizeof(I_Arena_Allocator);
  const size_t total_size = struct_size + arena_size;
  void* buf = malloc(total_size);
  if (buf == NULL) {
    return NULL;
  }
  I_Arena_Allocator* arena = (I_Arena_Allocator*)buf;
  arena->buf = (size_t*)buf + struct_size;
  arena->size = arena_size;
  arena->idx = 0;
  arena->type_enum = ARENA_TYPE_ENUM_STATIC;
  arena->arena_ctx = NULL;
  arena->alloc = alloc_static_arena;
  arena->free_arena = free_static_arena;
  return arena;
}

typedef struct Fixed_Bump_Arena_Context {
  unsigned int block_idx;
  void* data_blocks[FIXED_BUMP_ARENA_NUM_MAX_ALLOCATIONS];
} Fixed_Bump_Arena_Context;

void* alloc_fixed_bump_arena(I_Arena_Allocator* arena, const size_t size) {
  if (size > arena->size) {
    return NULL;
  }
  // TODO : > or >= ?
  if ((arena->idx + size) > arena->size) {
    assert(arena->type_enum == ARENA_TYPE_ENUM_FIXED_BUMP);
    Fixed_Bump_Arena_Context* arena_ctx = arena->arena_ctx;
    if (arena_ctx->block_idx >= FIXED_BUMP_ARENA_NUM_MAX_ALLOCATIONS) {
      return NULL;
    }

    void* new_data_block = malloc(arena->size);
    if (new_data_block == NULL) {
      return NULL;
    }
    arena_ctx->block_idx += 1;
    arena_ctx->data_blocks[arena_ctx->block_idx] = new_data_block;
    arena->buf = new_data_block;
    arena->idx = 0;
  }
  void* buf = (size_t*)arena->buf + size;
  arena->idx += size;
  return buf;
}

void free_fixed_bump_arena(I_Arena_Allocator* arena) {
  assert(arena->type_enum == ARENA_TYPE_ENUM_FIXED_BUMP);
  Fixed_Bump_Arena_Context* arena_ctx = arena->arena_ctx;
  assert(arena_ctx->block_idx < FIXED_BUMP_ARENA_NUM_MAX_ALLOCATIONS);
  for (unsigned int i = 0; i <= arena_ctx->block_idx; i++) {
    free(arena_ctx->data_blocks[i]);
  }
  free(arena);
}

I_Arena_Allocator* create_fixed_bump_arena(const size_t data_block_size) {
  const size_t struct_size = sizeof(I_Arena_Allocator);
  const size_t context_size = sizeof(Fixed_Bump_Arena_Context);
  void* struct_buf = malloc(struct_size + context_size);
  if (struct_buf == NULL) {
    return NULL;
  }
  I_Arena_Allocator* arena = (I_Arena_Allocator*)struct_buf;
  arena->arena_ctx = (size_t*)struct_buf + struct_size;
  Fixed_Bump_Arena_Context* arena_ctx = arena->arena_ctx;
  arena->size = data_block_size;
  arena->idx = 0;
  arena->type_enum = ARENA_TYPE_ENUM_FIXED_BUMP;
  arena->alloc = alloc_fixed_bump_arena;
  arena->free_arena = free_fixed_bump_arena;

  void* data_buf = malloc(data_block_size);
  if (data_buf == NULL) {
    free(struct_buf);
    return NULL;
  }
  arena->buf = data_buf;
  arena_ctx->block_idx = 0;
  arena_ctx->data_blocks[0] = data_buf;
  return arena;
}

#endif  // _BG_ARENA_C
