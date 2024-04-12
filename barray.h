#ifndef CLIB_BARRAY_H
#define CLIB_BARRAY_H

#include "clib.h"

#define BARRAY_PRINT_ELEMC 8

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
isize  barray_push(BArray* arr, void* data);
void   barray_free(BArray* arr);
void   barray_print(BArray* arr);

/* -------------------------------------------------------------------- */

#ifdef CLIB_BARRAY_IMPLEMENTATION

BArray barray_new(isize bucket_sz, isize elem_sz)
{
	ASSERT(bucket_sz, >= 0);
	ASSERT(elem_sz, > 0);

	DEFAULT(bucket_sz, CLIB_DEFAULT_BUCKET_SIZE);
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

isize barray_push(BArray* arr, void* data)
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

void barray_print(BArray* arr)
{
	fprintf(DEBUG_OUTPUT, "Bucket Array [%ldB per bucket | %ldB per element]:\n", arr->bucket_sz, arr->elem_sz);
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
}

#endif /* CLIB_BARRAY_IMPLEMENTATION */
#endif /* CLIB_BARRAY_H */

