#include "../clib.h"

void test_barray()
{
	int64 a = 3;
	int64 b = 5;
	int64 c = 7;

	INFO(" --- Testing BArray --- ");
	BArray arr = barray_new(0, 8);
	barray_print(&arr);
	barray_push(&arr, &a);
	barray_push(&arr, &b);
	barray_push(&arr, &c);
	barray_print(&arr);

	for (isize i = 0; i < 10000; i++)
		barray_push(&arr, &i);
	barray_print(&arr);

	barray_free(&arr);
	INFO(" --- Done Testing BArray --- ");
}

