#ifndef CLIB_ARENA_H
#define CLIB_ARENA_H

#include "common.h"

#define ARENA_RESIZE_FACTOR 2
#define ARENA_ALIGN_TO      8
#define ARENA_ALIGN(x)      ((x) += (ARENA_ALIGN_TO - ((x) % ARENA_ALIGN_TO)) % ARENA_ALIGN_TO)

enum ArenaFlag {
	ARENA_RESIZEABLE = 1 << 0,
	ARENA_NO_ALIGN   = 1 << 1,
};

struct Arena {
	enum ArenaFlag flags;
	isize cap;
	byte* top;
	byte* data;
};

struct Arena arena_new(isize sz, enum ArenaFlag flags);
void* arena_alloc(struct Arena* arena, isize sz);
void  arena_reset(struct Arena* arena);
void  arena_free(struct Arena* arena);

/* -------------------------------------------------------------------- */

#ifdef CLIB_ARENA_IMPLEMENTATION

struct Arena arena_new(isize sz, enum ArenaFlag flags)
{
	ASSERT(sz, > 0);

	if (!(flags & ARENA_NO_ALIGN))
		ARENA_ALIGN(sz);

	struct Arena arena = {
		.cap   = sz,
		.flags = flags,
		.data  = smalloc(sz),
	};
	arena.top = arena.data;

	CLIB_INFO(TERM_BLUE "[CLIB] Created new arena of size %ldB/%ldkB/%ldMB", sz, sz/1024, sz/1024/1024);
	return arena;
}

void* arena_alloc(struct Arena* arena, isize in_sz)
{
	ASSERT(in_sz, > 0);

	isize sz = in_sz;
	if (!(arena->flags & ARENA_NO_ALIGN))
		ARENA_ALIGN(sz);

	isize avail = arena->cap - (arena->top - arena->data);
	if (sz > avail) {
		if (arena->flags & ARENA_RESIZEABLE) {
			arena->cap  = MAX(arena->cap*ARENA_RESIZE_FACTOR, sz);
			arena->data = srealloc(arena->data, arena->cap);
			INFO(TERM_BLUE "[CLIB] Arena resized to %ldB", arena->cap);
		} else {
			ERROR("[CLIB] Arena out of memory.\n\tAvailable: %ldB of %ldB\n\trequested: %ldB (ARENA_RESIZEABLE not set)",
			      avail, arena->cap, in_sz);
			return NULL;
		}
	}

	arena->top += sz;
	return arena->top - sz;
}

void arena_reset(struct Arena* arena)
{
	arena->top = arena->data;
}

void arena_free(struct Arena* arena)
{
	sfree(arena->data);
}

#endif /* CLIB_ARENA_IMPLEMENTATION */
#endif /* CLIB_ARENA_H */

