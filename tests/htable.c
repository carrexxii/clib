#include "../common.h"

void test_htable()
{
	INFO(" --- Testing HTable ---");

	INFO("\n - - - - - - - * htable_insert * - - - - - - - ");
	struct HTable htable = htable_new(16, NULL);
	htable_insert(&htable, STRING("x"), 1);
	htable_insert(&htable, STRING("y"), 2);
	htable_insert(&htable, STRING("table"), 3);
	htable_insert(&htable, STRING("hash"), 4);
	htable_insert(&htable, STRING("longer_value"), 5);
	htable_insert(&htable, STRING("another_value"), 6);
	htable_insert(&htable, STRING("hello"), 7);
	htable_insert(&htable, STRING("world"), 8);
	htable_insert(&htable, STRING("x2"), 9);
	htable_insert(&htable, STRING("y2"), 10);
	htable_print(&htable);

	INFO("\n - - - - - - - * htable_print * - - - - - - - ");

	assert(1  == htable_get(&htable, STRING("x")));
	assert(2  == htable_get(&htable, STRING("y")));
	assert(3  == htable_get(&htable, STRING("table")));
	assert(4  == htable_get(&htable, STRING("hash")));
	assert(5  == htable_get(&htable, STRING("longer_value")));
	assert(6  == htable_get(&htable, STRING("another_value")));
	assert(7  == htable_get(&htable, STRING("hello")));
	assert(8  == htable_get(&htable, STRING("world")));
	assert(9  == htable_get(&htable, STRING("x2")));
	assert(10 == htable_get(&htable, STRING("y2")));

	INFO("\n - - - - - - - * htable_set * - - - - - - - ");
	INFO("Setting x and y to 50 and x2 and y2 to 100. Changing hash to 30 using htable_insert");
	htable_set(&htable, STRING("x"), 50);
	htable_set(&htable, STRING("y"), 50);
	htable_set(&htable, STRING("x2"), 100);
	htable_set(&htable, STRING("y2"), 100);
	htable_insert(&htable, STRING("hash"), 30);

	assert(50  == htable_get(&htable, STRING("x")));
	assert(50  == htable_get(&htable, STRING("y")));
	assert(100 == htable_get(&htable, STRING("x2")));
	assert(100 == htable_get(&htable, STRING("y2")));
	assert(30  == htable_get(&htable, STRING("hash")));

	INFO("\n - - - - - - - * htable_print * - - - - - - - ");
	htable_print(&htable);

	htable_free(&htable);
	INFO(" --- End of Testing HTable ---");
}

