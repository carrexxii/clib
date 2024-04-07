#ifndef CLIB_HTABLE_H
#define CLIB_HTABLE_H

#include "common.h"

/* TODO:
 *  - Add a resize function and/or copy function
 *  - Add a dynamic array for storing linked list elements and strings (arena allocator?)
 */

struct HPair {
	char* key;
	int64 val;
	struct HPair* next;
};

struct HTable {
	struct Arena arena;
	bool arena_is_external;
	isize cap;
	struct HPair* pairs;
};

struct HTable htable_new(isize count, struct Arena* arena);
uint64        htable_hash(struct HTable* restrict htable, char* restrict str);
struct HPair* htable_insert(struct HTable* restrict htable, char* restrict key, int64 val);
struct HPair* htable_get_pair(struct HTable* htable, char* key);
int64         htable_get(struct HTable* restrict htable, char* restrict key);
int64         htable_get_or_insert(struct HTable* restrict htable, char* restrict key, int64 val);
int           htable_set(struct HTable* restrict htable, char* restrict key, int64 val);
void          htable_print(struct HTable* htable);
void          htable_free(struct HTable* htable);

/* -------------------------------------------------------------------- */

#ifdef CLIB_HTABLE_IMPLEMENTATION

struct HTable htable_new(isize count, struct Arena* arena)
{
	ASSERT(count, > 0);

	return (struct HTable){
		.arena = arena? *arena: arena_new(CLIB_HTABLE_SIZE_PER_ELEMENT*count, ARENA_RESIZEABLE),
		.cap   = count,
		.pairs = scalloc(count, sizeof(struct HPair)),
		.arena_is_external = !arena,
	};

	CLIB_INFO(TERM_BLUE "[CLIB] Created new htable with %ld elements (arena: %s)", count, STR_TF(arena));
}

/* PJW Hash Function: https://www.partow.net/programming/hashfunctions/ */
uint64 htable_hash(struct HTable* restrict htable, char* restrict str)
{
	uint64 hash = 1315423911;
	char* c = str;
	while (*c)
		hash ^= ((hash << 5) + (*c++) + (hash >> 2));

	return hash % htable->cap;
}

struct HPair* htable_insert(struct HTable* restrict htable, char* restrict key, int64 val)
{
	assert(key);

	struct HPair* pair = &htable->pairs[htable_hash(htable, key)];
	/* Only search through the list if first slot is not available */
	if (pair->key) {
		do {
			/* Check for duplicate keys */
			if (!pair->key || !strcmp(pair->key, key))
				break;
			if (!pair->next) {
				pair->next = arena_alloc(&htable->arena, sizeof(struct HPair));
				pair = pair->next;
				*pair = (struct HPair){ 0 };
				break;
			}
			pair = pair->next;
		} while (pair);
	}
	if (!pair->key) {
		isize len = strlen(key);
		pair->key = arena_alloc(&htable->arena, (len + 1)*sizeof(char));
		strcpy(pair->key, key);
		pair->key[len] = '\0';
	}
	pair->val = val;

	return pair;
}

/* Returns NULL if the key was not found */
struct HPair* htable_get_pair(struct HTable* restrict htable, char* restrict key)
{
	struct HPair* pair = &htable->pairs[htable_hash(htable, key)];
	if (!pair->key)
		return NULL;

	while (strcmp(pair->key, key)) {
		pair = pair->next;
		if (!pair)
			return NULL;
	}

	return pair;
}

/* Returns -1 if the key was not found */
int64 htable_get(struct HTable* restrict htable, char* restrict key)
{
	struct HPair* pair = htable_get_pair(htable, key);
	return pair? pair->val
	           : -1;
}

/* Same as htable_get, but will insert the value if the key is not present */
int64 htable_get_or_insert(struct HTable* restrict htable, char* restrict key, int64 val)
{
	struct HPair* pair = htable_get_pair(htable, key);
	return pair? pair->val
	           : htable_insert(htable, key, val)->val;
}

/* Returns 0 if the value was set or -1 if the key was not found */
int htable_set(struct HTable* restrict htable, char* restrict key, int64 val)
{
	// TODO: This should insert if its not there (ie, combine with htable_insert())
	struct HPair* pair = htable_get_pair(htable, key);
	if (pair)
		pair->val = val;
	else
		return -1;

	return 0;
}

void htable_print(struct HTable* htable)
{
	fprintf(DEBUG_OUTPUT, TERM_BLUE "[CLIB] Hashtable (capacity: %ld):\n" TERM_NORMAL, htable->cap);
	struct HPair* pair;
	for (int i = 0; i < htable->cap; i++) {
		pair = &htable->pairs[i];
		fprintf(DEBUG_OUTPUT, "\t[%s: %ld]", pair->key? pair->key: "", pair->val);
		while (pair->next) {
			pair = pair->next;
			fprintf(DEBUG_OUTPUT, " -> [%s: %ld]", pair->key, pair->val);
		}
		fprintf(DEBUG_OUTPUT, "\n");
	}
}

void htable_free(struct HTable* htable)
{
	if (htable->arena_is_external)
		arena_free(&htable->arena);
	sfree(htable->pairs);
}

#endif /* CLIB_HTABLE_IMPLEMENTATION */
#endif /* CLIB_HTABLE_H */

