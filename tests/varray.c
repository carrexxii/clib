#include "../clib.h"

void test_varray()
{
	INFO(" --- Testing VArray ---");
	struct VArray arr = varray_new(10, 4);
	varray_print(&arr);
	varray_push(&arr, (int[]){ 1 });
	varray_push(&arr, (int[]){ 2 });
	varray_push(&arr, (int[]){ 3 });
	varray_push(&arr, (int[]){ 4 });
	varray_push(&arr, (int[]){ 5 });
	varray_print(&arr);

	INFO("%d", *(int*)varray_get(&arr, 0));
	INFO("%d", *(int*)varray_get(&arr, 1));
	INFO("%d", *(int*)varray_get(&arr, 2));
	INFO("%d", *(int*)varray_get(&arr, 3));
	INFO("%d", *(int*)varray_get(&arr, 4));

	INFO("Setting [0] = 99 and [4] = 100");
	varray_set(&arr, 0, (int[]){ 99 });
	varray_set(&arr, 4, (int[]){ 100 });
	varray_print(&arr);

	varray_free(&arr);
}

