#ifndef CLIB_BARRAY_H
#define CLIB_BARRAY_H

#include "clib.h"

#define BARRAY_PRINT_ELEMC 8

#define PPCAT2(a, b) a ## b
#define PPCAT(a, b)  PPCAT2(a, b)
#define BARRAY_ITER(_arr, _v)                                                           \
	int PPCAT(_i, __LINE__) = 0;                                                         \
	Bucket* PPCAT(_b, __LINE__) = (_arr).head;                                            \
	for (_v = (void*)PPCAT(_b, __LINE__)->elems;                                           \
	     (_v = (void*)(PPCAT(_b, __LINE__)->elems + PPCAT(_i, __LINE__)*_arr.elem_sz)) &&   \
		  PPCAT(_i, __LINE__) < PPCAT(_b, __LINE__)->elemc;                                  \
	     (PPCAT(_i, __LINE__) == PPCAT(_b, __LINE__)->elemc - 1 && PPCAT(_b, __LINE__)->next) \
			? ((PPCAT(_i, __LINE__) = 0) && (PPCAT(_b, __LINE__) = PPCAT(_b, __LINE__)->next)) \
			: PPCAT(_i, __LINE__)++)

typedef struct Bucket {
	struct Bucket* next;
	isize elemc;
	byte elems[];
} Bucket;

typedef struct BArray {
	isize bucket_sz;
	isize elem_sz;
	isize elems_per_bucket;
	Bucket* head;
} BArray;

BArray barray_new(isize bucket_sz, isize elem_sz);
void   barray_alloc_bucket(BArray* arr);
isize  barray_push(BArray* restrict arr, const void* restrict data);
void*  barray_pop(BArray* arr);
void*  barray_get(const BArray* arr, isize i);
void*  barray_set(BArray* restrict arr, isize i, const void* restrict data);
void   barray_free(BArray* arr);
void   barray_print(const BArray* arr);

/* -------------------------------------------------------------------- */

#ifdef CLIB_BARRAY_IMPLEMENTATION

BArray barray_new(isize bucket_sz, isize elem_sz)
{
	ASSERT(bucket_sz, >= 0);
	ASSERT(elem_sz, > 0);

	DEFAULT(bucket_sz, CLIB_DEFAULT_BUCKET_SIZE);
	ASSERT(bucket_sz, > elem_sz);

	BArray arr = {
		.bucket_sz        = bucket_sz,
		.elem_sz          = elem_sz,
		.elems_per_bucket = bucket_sz / elem_sz,
	};

	arr.head = smalloc(sizeof(Bucket) + arr.bucket_sz);
	arr.head->elemc = 0;
	arr.head->next  = NULL;

	CLIB_INFO(TERM_BLUE "[CLIB] Created new bucket array with bucket size %ldB, %ldB per element = %ld elements per bucket",
	          arr.bucket_sz + sizeof(Bucket), arr.elem_sz, arr.elems_per_bucket);
	return arr;
}

void barray_alloc_bucket(BArray* arr)
{
	Bucket* b = arr->head;
	while (b->next)
		b = b->next;

	b->next = smalloc(sizeof(Bucket) + arr->bucket_sz);
	b->next->elemc = 0;
	b->next->next  = NULL;
}

isize barray_push(BArray* restrict arr, const void* restrict data)
{
	intptr gi = 0;
	Bucket* b = arr->head;
	while (b->elemc >= arr->elems_per_bucket) {
		if (!b->next)
			barray_alloc_bucket(arr);
		b = b->next;
		gi += arr->elems_per_bucket;
	}

	memcpy(b->elems + b->elemc*arr->elem_sz, data, arr->elem_sz);
	return b->elemc++;
}

void* barray_pop(BArray* arr)
{
	Bucket* b = arr->head;
	while (b->next && b->next->elemc)
		b = b->next;

	b->elemc--;
	return b->elems + b->elemc*arr->elem_sz;
}

void* barray_get(const BArray* arr, isize i)
{
	ASSERT(i, >= 0);

	Bucket* b = arr->head;
	isize li = i;
	while (b) {
		if (li < b->elemc)
			return b->elems + li*arr->elem_sz;
		li -= b->elemc;
		b = b->next;
	}

	ERROR("[CLIB] Index %ld is out of bounds for bucket array", i);
	return NULL;
}

void* barray_set(BArray* restrict arr, isize i, const void* restrict data)
{
	ASSERT2(i, >= 0, data, != NULL);

	void* elem = barray_get(arr, i);
	memcpy(elem, data, arr->elem_sz);

	return elem;
}

void barray_free(BArray* arr)
{
	Bucket* curr = arr->head;
	Bucket* prev;
	while (curr) {
		prev = curr;
		curr = curr->next;
		sfree(prev);
	}
}

void barray_print(const BArray* arr)
{
	fprintf(DEBUG_OUTPUT, TERM_BLUE "[CLIB] Bucket Array [%ldB per bucket | %ldB per element]:\n",
	        arr->bucket_sz, arr->elem_sz);
	Bucket* b = arr->head;
	for (int i = 0; b; b = b->next, i++) {
		fprintf(DEBUG_OUTPUT, "\tBucket %d -> %ld/%ld [", i, b->elemc, arr->elems_per_bucket);
		for (int j = 0; j < MIN(b->elemc, BARRAY_PRINT_ELEMC); j++) {
			fprintf(DEBUG_OUTPUT, "%ld", ((int64*)b->elems)[j]);
			if (j < b->elemc - 1)
				fprintf(DEBUG_OUTPUT, ", ");
		}
		fprintf(DEBUG_OUTPUT, "%s]\n", b->elemc > BARRAY_PRINT_ELEMC? "... ": "");
	}
	fprintf(DEBUG_OUTPUT, TERM_NORMAL);
}

#endif /* CLIB_BARRAY_IMPLEMENTATION */
#endif /* CLIB_BARRAY_H */

