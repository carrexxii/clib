#ifndef CLIB_IARRAY_H
#define CLIB_IARRAY_H

#include "clib.h"

#define DEFAULT_IARRAY_SIZE    10
#define IARRAY_SIZE_MULTIPLIER 1.5

typedef struct IArray {
	intptr  sz;
	intptr  itemc;
	intptr  itemsz;
	bool    sorted;
	uint16* inds;
	void*   data;
} IArray;

IArray iarr_new(intptr itemsz, intptr sz);
void   iarr_resize(IArray* arr, intptr itemc);
void*  iarr_append(IArray* arr, intptr i, void* data);
void*  iarr_get(IArray arr, intptr i);
void   iarr_print(IArray arr);
void   iarr_free(IArray* arr, void (*cb)(void*));

/* -------------------------------------------------------------------- */

#ifdef CLIB_IARRAY_IMPLEMENTATION

inline static intptr linear_search(intptr indc, uint16* inds, intptr key);
inline static intptr bin_search(intptr indc, uint16* inds, intptr key);

IArray iarr_new(intptr itemsz, intptr sz)
{
	IArray arr = {
		.sz     = sz? sz: DEFAULT_IARRAY_SIZE,
		.itemsz = itemsz,
		.sorted = true,
	};
	iarr_resize(&arr, arr.sz);

	CLIB_INFO(TERM_BLUE "[CLIB] Created IArray with %ld items of %ldB = %ldkB/%ldMB", sz, itemsz, sz*itemsz/1024, sz*itemsz/1024/1024);
	return arr;
}

void iarr_resize(IArray* arr, intptr sz)
{
	arr->inds = srealloc(arr->inds, sz*(sizeof(uint16) + arr->itemsz));
	arr->sz   = sz;
	arr->data = arr->inds + sz;
}

void* iarr_append(IArray* arr, intptr i, void* data)
{
	if (arr->itemc >= arr->sz)
		iarr_resize(arr, arr->sz*IARRAY_SIZE_MULTIPLIER);

	arr->inds[arr->itemc] = i;
	void* mem = (byte*)arr->data + arr->itemc*arr->itemsz;
	memcpy(mem, data, arr->itemsz);
	if (arr->itemc && i < arr->inds[arr->itemc - 1])
		arr->sorted = false;
	arr->itemc++;

	return mem;
}

/* Don't think this works */
void* iarr_get(IArray arr, intptr i)
{
	intptr arri;
	if (arr.sorted)
		arri = bin_search(arr.itemc, arr.inds, i);
	else
		arri = linear_search(arr.itemc, arr.inds, i);

	if (arri >= 0)
		return (byte*)arr.data + arri*arr.itemsz;
	else
		return NULL;
}

inline static intptr linear_search(intptr indc, uint16* inds, intptr key)
{
	for (int i = 0; i < indc; i++)
		if (inds[i] == key)
			return i;

	return -1;
}

inline static intptr bin_search(intptr indc, uint16* inds, intptr key)
{
	uint l, u, k, i; /* lower, upper, key, index */
	l = 0;
	u = indc - 1;
	while (l <= u) {
		i = (l + u) / 2;
		k = inds[i];
		if (k == key)
			return i;
		else if (k > key)
			u = i - 1;
		else
			l = i + 1;
	}

	return -1;
}

void iarr_print(IArray arr)
{
	fprintf(DEBUG_OUTPUT, TERM_BLUE "IArray:\n\t");
	for (int i = 0; i < arr.itemc; i++) {
		fprintf(DEBUG_OUTPUT, "%2hu ", arr.inds[i]);
		if (!((i + 1) % 20))
			fprintf(DEBUG_OUTPUT, "\n\t");
	}
	fprintf(DEBUG_OUTPUT, "\n" TERM_NORMAL);
}

void iarr_free(IArray* arr, void (*cb)(void*))
{
	if (cb)
		for (int i = 0; i < arr->itemc; i++)
			if (arr->inds[i])
				cb((byte*)arr->data + i*arr->itemsz);
	free(arr->inds); /* This also frees the data */
}

#endif /* CLIB_IARRAY_IMPLEMENTATION */
#endif /* CLIB_IARRAY_H */

