#ifndef CLIB_HTABLE_H
#define CLIB_HTABLE_H

#include "clib.h"

/* TODO:
 *  - Add a resize function and/or copy function
 *  - Add a dynamic array for storing linked list elements and strings (arena allocator?)
 */

typedef struct HPair {
	String key;
	intptr val;
	struct HPair* next;
} HPair;

typedef struct HTable {
	Arena arena;
	isize cap;
	HPair* pairs;
} HTable;

HTable htable_new(isize count);
uint64 htable_hash(HTable* htable, String str);
HPair* htable_insert(HTable* htable, String key, intptr val);
HPair* htable_get_pair(HTable* htable, String key);
intptr htable_get(HTable* htable, String key);
intptr htable_get_or_insert(HTable* htable, String key, intptr val);
int    htable_set(HTable* htable, String key, intptr val);
void   htable_print(HTable* htable);
void   htable_free(HTable* htable);

/* -------------------------------------------------------------------- */

#ifdef CLIB_HTABLE_IMPLEMENTATION

HTable htable_new(isize count)
{
	ASSERT(count, > 0);

	return (HTable){
		.arena = arena_new(CLIB_HTABLE_SIZE_PER_ELEMENT*count, ARENA_RESIZEABLE),
		.cap   = count,
		.pairs = scalloc(count, sizeof(HPair)),
	};

	CLIB_INFO(TERM_BLUE "[CLIB] Created new htable with %ld elements", count);
}

/* PJW Hash Function: https://www.partow.net/programming/hashfunctions/ */
uint64 htable_hash(HTable* htable, String str)
{
	uint64 hash = 1315423911;
	char* c = str.data;
	while (*c)
		hash ^= ((hash << 5) + (*c++) + (hash >> 2));

	return hash % htable->cap;
}

HPair* htable_insert(HTable* htable, String key, intptr val)
{
	HPair* pair = &htable->pairs[htable_hash(htable, key)];
	/* Only search through the list if first slot is not available */
	if (pair->key.data) {
		do {
			/* Check for duplicate keys */
			if (!pair->key.data || !strcmp(pair->key.data, key.data))
				break;
			if (!pair->next) {
				pair->next = arena_alloc(&htable->arena, sizeof(HPair));
				pair = pair->next;
				*pair = (HPair){ 0 };
				break;
			}
			pair = pair->next;
		} while (pair);
	}
	if (!pair->key.data)
		pair->key = string_copy(key, &htable->arena);
	pair->val = val;

	return pair;
}

/* Returns NULL if the key was not found */
HPair* htable_get_pair(HTable* htable, String key)
{
	HPair* pair = &htable->pairs[htable_hash(htable, key)];
	if (!pair->key.data)
		return NULL;

	while (strcmp(pair->key.data, key.data)) {
		pair = pair->next;
		if (!pair)
			return NULL;
	}

	return pair;
}

/* Returns -1 if the key was not found */
intptr htable_get(HTable* htable, String key)
{
	HPair* pair = htable_get_pair(htable, key);
	return pair? pair->val
	           : -1;
}

/* Same as htable_get, but will insert the value if the key is not present */
intptr htable_get_or_insert(HTable* htable, String key, intptr val)
{
	HPair* pair = htable_get_pair(htable, key);
	return pair? pair->val
	           : htable_insert(htable, key, val)->val;
}

/* Returns 0 if the value was set or -1 if the key was not found */
int htable_set(HTable* htable, String key, intptr val)
{
	// TODO: This should insert if its not there (ie, combine with htable_insert())
	HPair* pair = htable_get_pair(htable, key);
	if (pair)
		pair->val = val;
	else
		return -1;

	return 0;
}

void htable_print(HTable* htable)
{
	fprintf(DEBUG_OUTPUT, TERM_BLUE "[CLIB] Hashtable (capacity: %ld):\n" TERM_NORMAL, htable->cap);
	HPair* pair;
	for (int i = 0; i < htable->cap; i++) {
		pair = &htable->pairs[i];
		fprintf(DEBUG_OUTPUT, "\t[%s: %ld]", pair->key.data? pair->key.data: "", pair->val);
		while (pair->next) {
			pair = pair->next;
			fprintf(DEBUG_OUTPUT, " -> [%s: %ld]", pair->key.data, pair->val);
		}
		fprintf(DEBUG_OUTPUT, "\n");
	}
}

void htable_free(HTable* htable)
{
	arena_free(&htable->arena);
	sfree(htable->pairs);
}

#endif /* CLIB_HTABLE_IMPLEMENTATION */
#endif /* CLIB_HTABLE_H */

