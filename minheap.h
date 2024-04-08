#ifndef CLIB_MINHEAP_H
#define CLIB_MINHEAP_H

#include "common.h"

// TODO: MaxHeap or generalize to BinHeap

#define BUFFER_SIZE   128
#define NODE_COUNT(d) ((1 << (d)) - 1)

typedef struct MinHeap {
	int*  nodes;
	byte* data;
	isize elem_sz;
	isize depth;
	isize tail;
} MinHeap;

static inline void minheap_resize(MinHeap* heap, isize depth)
{
	int nodec = NODE_COUNT(depth);
	if (depth > heap->depth) {
		heap->depth = depth;
		heap->nodes = srealloc(heap->nodes, nodec*sizeof(*heap->nodes));
		heap->data  = srealloc(heap->data, (nodec + 1)*heap->elem_sz); /* +1 For returning data */
	} else {
		ERROR("[CLIB] Invalid new heap depth: got %ld when at %ld", depth, heap->depth);
	}
}

static inline MinHeap minheap_new(int depth, int elem_sz)
{
	MinHeap heap = {
		.elem_sz = elem_sz,
		.tail    = -1,
		.nodes   = NULL,
		.data    = NULL,
	};
	minheap_resize(&heap, depth);

	INFO(TERM_BLUE "Created new MinHeap with depth %d (%d nodes of size %dB)", depth, NODE_COUNT(depth), elem_sz);
	return heap;
}

static inline int minheap_parent(int i) { return (i - 1)/2; }
static inline int minheap_left(int i)   { return 2*i + 1;   }
static inline int minheap_right(int i)  { return 2*i + 2;   }
static inline void* minheap_get(MinHeap* heap, int i) {
	return heap->data + i*heap->elem_sz;
}
static inline void minheap_swap(MinHeap* heap, int i1, int i2)
{
	if (i1 == i2)
		return;
	int tmp = heap->nodes[i1];
	heap->nodes[i1] = heap->nodes[i2];
	heap->nodes[i2] = tmp;

	byte tmp_buf[BUFFER_SIZE];
	memcpy(tmp_buf, minheap_get(heap, i1), heap->elem_sz);
	memcpy(minheap_get(heap, i1), minheap_get(heap, i2), heap->elem_sz);
	memcpy(minheap_get(heap, i2), tmp_buf, heap->elem_sz);
}

static inline int minheap_min(MinHeap* heap) { return heap->nodes[0]; }
static inline void* minheap_pop(MinHeap* heap)
{
	void* data = minheap_get(heap, NODE_COUNT(heap->depth));
	memcpy(data, heap->data, heap->elem_sz);

	int l, r, i = 0;
	minheap_swap(heap, 0, heap->tail);
	while (i <= heap->tail) {
		l = heap->nodes[minheap_left(i)];
		r = heap->nodes[minheap_right(i)];
		if (l < r && heap->nodes[i] < l) {
			minheap_swap(heap, i, minheap_left(i));
			i = minheap_left(i);
		} else if (r <= l && heap->nodes[i] < r) {
			minheap_swap(heap, i, minheap_right(i));
			i = minheap_right(i);
		} else {
			break;
		}
	}
	heap->tail--;

	return data;
}

static inline int minheap_push(MinHeap* heap, int val, void* data)
{
	int i = ++heap->tail;
	if (i >= NODE_COUNT(heap->depth))
		minheap_resize(heap, heap->depth + 1);

	heap->nodes[i] = val;
	memcpy(minheap_get(heap, i), data, heap->elem_sz);
	while (i && heap->nodes[i] < heap->nodes[minheap_parent(i)]) {
		minheap_swap(heap, i, minheap_parent(i));
		i = minheap_parent(i);
	}

	return i;
}

static inline void* minheap_contains_data(MinHeap* heap, void* data)
{
	for (int i = 0; i <= heap->tail; i++)
		if (!memcmp(minheap_get(heap, i), data, heap->elem_sz))
			return minheap_get(heap, i);

	return NULL;
}

static inline int  minheap_size(MinHeap* heap)     { return heap->tail + 1; }
static inline bool minheap_is_empty(MinHeap* heap) { return heap->tail < 0; }
static inline void minheap_reset(MinHeap* heap)    { heap->tail = 0;        }

static inline void minheap_free(MinHeap* heap, void (*fn)(void*)) {
	sfree(heap->nodes);
	if (fn)
		for (int i = 0; i < heap->tail; i++)
			fn(minheap_get(heap, i));
	sfree(heap->data);
}

static void minheap_print(MinHeap* heap)
{
	INFO(TERM_BLUE "MinHeap with %d nodes (tail: %ld)", NODE_COUNT(heap->depth), heap->tail);
	isize li, ri, l, r;
	for (isize i = 0; i < NODE_COUNT(heap->depth); i++) {
		li = minheap_left(i);
		ri = minheap_right(i);
		l  = heap->nodes[li];
		r  = heap->nodes[ri];
		fprintf(DEBUG_OUTPUT, TERM_BLUE "[%ld -> %ld|%ld] \t %d -> %ld|%ld \n", i, li, ri, i <= heap->tail? heap->nodes[i]: -1,
		        l && li <= heap->tail? l: -1, r && ri <= heap->tail? r: -1);
	}
	fprintf(DEBUG_OUTPUT, "\n" TERM_NORMAL);
}

#endif

