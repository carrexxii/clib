#include "../clib.h"

void test_arena()
{
	INFO(" --- Testing Arena ---");
	struct Arena a1 = arena_new(128, 0);
	struct Arena a2 = arena_new(299, ARENA_RESIZEABLE);
	struct Arena a3 = arena_new(64, 0);

	assert(arena_alloc(&a1, 64));
	assert(arena_alloc(&a1, 30));
	// assert(!arena_alloc(&a1, 36));
	// assert(!arena_alloc(&a1, 64));
	arena_reset(&a1);
	assert(arena_alloc(&a1, 128));

	assert(arena_alloc(&a2, 100));
	assert(arena_alloc(&a2, 150));
	assert(arena_alloc(&a2, 200));

	arena_free(&a1);
	arena_free(&a2);
	arena_free(&a3);
	INFO(" --- Done Testing Arena ---");
}

