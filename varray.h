#ifndef CLIB_VARRAY_H
#define CLIB_VARRAY_H

#include "common.h"

// TODO: Add allocator parameters
// TODO: VLS?
struct VArray {
	isize len;
	isize cap;
	isize elem_sz;
	byte* data;
};

struct VArray varray_new(isize elemc, isize elem_sz);
void* varray_set(struct VArray* restrict arr, isize i, void* restrict elem);
void* varray_get(struct VArray* arr, isize i);
isize varray_push(struct VArray* restrict arr, void* restrict data);
isize varray_push_many(struct VArray* restrict arr, isize count, void* restrict elems);
isize varray_push_many_strided(struct VArray* restrict arr, isize elemc, void* restrict elems, isize offset, isize stride);
void* varray_pop(struct VArray* arr);
bool  varray_contains(struct VArray* arr, void* data);
void  varray_resize(struct VArray* arr, isize new_len, bool shrink);
void  varray_free(struct VArray* arr);
void varray_print(struct VArray* arr);

/* -------------------------------------------------------------------- */

#ifdef CLIB_VARRAY_IMPLEMENTATION

// TODO: flags for slow grow/no grow/...
struct VArray varray_new(isize elemc, isize elem_sz)
{
	ASSERT(elemc, > 0);
	ASSERT(elem_sz, > 0);

	struct VArray arr = {
		.data    = smalloc(elemc*elem_sz),
		.len     = 0,
		.cap     = elemc,
		.elem_sz = elem_sz,
	};

	CLIB_INFO(TERM_BLUE "[CLIB] Created new VArray with %ld elements of size %ld", arr.cap, arr.elem_sz);
	return arr;
}

void* varray_set(struct VArray* arr, isize i, void* elem)
{
	assert(i >= 0 && i < arr->len);

	memcpy(arr->data + i*arr->elem_sz, elem, arr->elem_sz);

	return arr->data + i*arr->elem_sz;
}

void* varray_get(struct VArray* arr, isize i)
{
	assert(i >= 0 && i < arr->len);

	return arr->data + i*arr->elem_sz;
}

void* varray_pop(struct VArray* arr)
{
	assert(arr->len > 0);

	return arr->data + (--arr->len)*arr->elem_sz;
}

isize varray_push(struct VArray* restrict arr, void* restrict elem)
{
	varray_resize(arr, arr->len + 1, false);
	memcpy(arr->data + arr->len*arr->elem_sz, elem, arr->elem_sz);

	return arr->len++;
}

isize varray_push_many(struct VArray* restrict arr, isize elemc, void* restrict elems)
{
	assert(elemc > 0);

	varray_resize(arr, arr->len + elemc, false);
	memcpy(arr->data + arr->len*arr->elem_sz, elems, elemc*arr->elem_sz);
	arr->len += elemc;

	return arr->len - elemc;
}

isize varray_push_many_strided(struct VArray* restrict arr, isize elemc, void* restrict elems, isize offset, isize stride)
{
	assert(elemc > 0);

	varray_resize(arr, arr->len + elemc, false);
	byte* data = (byte*)elems + offset;
	for (int i = 0; i < elemc; i++) {
		memcpy(arr->data + arr->len*arr->elem_sz, data, arr->elem_sz);
		data += stride;
		arr->len++;
	}

	return arr->len - elemc;
}

bool varray_contains(struct VArray* arr, void* data)
{
	for (int i = 0; i < arr->len; i++)
		if (!memcmp(varray_get(arr, i), data, arr->elem_sz))
			return true;

	return false;
}

void varray_resize(struct VArray* arr, isize new_len, bool shrink)
{
	if (arr->cap < new_len || (arr->cap != new_len && shrink)) {
		arr->cap  = new_len;
		arr->data = srealloc(arr->data, arr->cap * arr->elem_sz);
		CLIB_INFO(TERM_BLUE "[CLIB] Resized VArray [len=%ld; cap=%ld; elem_sz=%ldB] = %ldB/%.2fkB/%.2fMB", arr->len, arr->cap, arr->elem_sz,
		          arr->cap*arr->elem_sz, arr->cap*arr->elem_sz/1024.0, arr->cap*arr->elem_sz/1024.0/1024.0);
	}
}

void varray_reset(struct VArray* arr)
{
	arr->len = 0;
}

void varray_free(struct VArray* arr)
{
	arr->cap = 0;
	arr->len = 0;
	sfree(arr->data);
}

void varray_print(struct VArray* arr)
{
	fprintf(DEBUG_OUTPUT, TERM_BLUE "[CLIB] VArray: (%ld elements of size %ld; %ld total capacity)", arr->len, arr->elem_sz, arr->cap);
	for (int i = 0; i < arr->cap; i++) {
		printf(i % 4 == 0? "\n": "");
		printf(TERM_BLUE "\t[%d: %2ld] %d", i, (byte*)(arr->data + i*arr->elem_sz) - arr->data, *(int*)(arr->data + i*arr->elem_sz));
		if (i == arr->len - 1)
			printf("*");
	}
	printf(TERM_NORMAL "\n");
}

#endif /* CLIB_VARRAY_IMPLEMENTATION */
#endif /* CLIB_VARRAY_H */

