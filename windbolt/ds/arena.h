#ifndef ARENA_H
#define ARENA_H

// NOTE(smoke): quick and dirty arena allocator
#include <stdlib.h>
#include <stddef.h>

// NOTE(smoke): user-provided malloc and free
#if !defined(ARENA_MALLOC)
# define ARENA_MALLOC malloc
#endif
#if !defined(ARENA_FREE)
# define ARENA_FREE free
#endif

// NOTE(smoke): (optional) custom assertion
#if 0
#if !defined(ARENA_ASSERT)
# include <stdio.h>
# define ARENA_ASSERT_BREAK() (*(int*)0 = 0)
# define ARENA_ASSERT_MESSAGE(msg) fprintf(stderr, "assertion failure: %s\n", msg);
# define ARENA_ASSERT(c,msg) if(!(c)){ARENA_ASSERT_MESSAGE((msg));ARENA_ASSERT_BREAK();}
#endif
#endif
#include <base.h>
#define ARENA_ASSERT ASSERT

// NOTE(smoke): the actual arena...
typedef struct Arena
{
    size_t offset, capacity;
    void *data;
} Arena;

Arena arena_create(size_t capacity);
void  arena_destroy(Arena *arena);
void* arena_alloc(Arena *arena, size_t size);
void  arena_reset(Arena *arena);

#endif // ARENA_H
