#include "../clib.h"

void test_barray()
{
	int64 a = 3;
	int64 b = 5;
	int64 c = 7;

	isize bucket_sz = 64*1024;

test_start:
	INFO("\n --- Testing BArray (bucket_sz = %ld) --- ", bucket_sz);
	BArray arr = barray_new(0, 8);

	INFO("\n --- barray_push --- ");
	barray_print(&arr);
	barray_push(&arr, &a);
	barray_push(&arr, &b);
	barray_push(&arr, &c);
	barray_push(&arr, &b);
	barray_push(&arr, &a);

	INFO("\n --- barray_get --- ");
	DV(*(int64*)barray_get(&arr, 0));
	DV(*(int64*)barray_get(&arr, 1));
	DV(*(int64*)barray_get(&arr, 2));
	DV(*(int64*)barray_get(&arr, 3));
	DV(*(int64*)barray_get(&arr, 4));
	barray_print(&arr);

	INFO("\n --- barray_set --- ");
	barray_set(&arr, 0, &c);
	barray_set(&arr, 1, &b);
	barray_set(&arr, 2, &a);
	barray_set(&arr, 3, &b);
	barray_set(&arr, 4, &c);
	barray_print(&arr);

	INFO("\n --- barray_pop --- ");
	barray_print(&arr);
	DV(*(int64*)barray_pop(&arr));
	DV(*(int64*)barray_pop(&arr));
	DV(*(int64*)barray_pop(&arr));
	DV(*(int64*)barray_pop(&arr));
	barray_print(&arr);

	for (isize i = 0; i < 10000; i++)
		barray_push(&arr, &i);
	barray_print(&arr);

	barray_free(&arr);
	bucket_sz /= 2;
	bucket_sz -= 64;
	if (bucket_sz >= 0)
		goto test_start;

	INFO("\n --- Done Testing BArray --- ");
}

