#include "arena.h"
#include <string.h>

Arena arena_create(size_t capacity)
{
    Arena arena = {0};
    
    arena.data = ARENA_MALLOC(capacity);
    ARENA_ASSERT(arena.data, "arena_create: ARENA_MALLOC has return NULL");
    arena_reset(&arena);
    
    arena.capacity = capacity;
    return arena;
}

void arena_destroy(Arena *arena)
{
    if (arena && arena->data)
        ARENA_FREE(arena->data);
}

void* arena_alloc(Arena *arena, size_t size)
{
    ARENA_ASSERT(arena->offset + size <= arena->capacity, "arena_alloc: out of space");
    
    void *ptr = arena->data + arena->offset;
    arena->offset += size;
    
    return ptr;
}

void arena_reset(Arena *arena)
{
    memset(arena->data, 0, arena->capacity);
}
