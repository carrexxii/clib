#include "../common.h"

void test_htable()
{
	INFO(" --- Testing HTable ---");

	INFO("\n - - - - - - - * htable_insert * - - - - - - - ");
	struct HTable htable = htable_new(16, NULL);
	htable_insert(&htable, "x", 1);
	htable_insert(&htable, "y", 2);
	htable_insert(&htable, "table", 3);
	htable_insert(&htable, "hash", 4);
	htable_insert(&htable, "longer_value", 5);
	htable_insert(&htable, "another_value", 6);
	htable_insert(&htable, "hello", 7);
	htable_insert(&htable, "world", 8);
	htable_insert(&htable, "x2", 9);
	htable_insert(&htable, "y2", 10);
	htable_print(&htable);

	INFO("\n - - - - - - - * htable_print * - - - - - - - ");

	assert(1  == htable_get(&htable, "x"));
	assert(2  == htable_get(&htable, "y"));
	assert(3  == htable_get(&htable, "table"));
	assert(4  == htable_get(&htable, "hash"));
	assert(5  == htable_get(&htable, "longer_value"));
	assert(6  == htable_get(&htable, "another_value"));
	assert(7  == htable_get(&htable, "hello"));
	assert(8  == htable_get(&htable, "world"));
	assert(9  == htable_get(&htable, "x2"));
	assert(10 == htable_get(&htable, "y2"));

	INFO("\n - - - - - - - * htable_set * - - - - - - - ");
	INFO("Setting x and y to 50 and x2 and y2 to 100. Changing hash to 30 using htable_insert");
	htable_set(&htable, "x", 50);
	htable_set(&htable, "y", 50);
	htable_set(&htable, "x2", 100);
	htable_set(&htable, "y2", 100);
	htable_insert(&htable, "hash", 30);

	assert(50  == htable_get(&htable, "x"));
	assert(50  == htable_get(&htable, "y"));
	assert(100 == htable_get(&htable, "x2"));
	assert(100 == htable_get(&htable, "y2"));
	assert(30  == htable_get(&htable, "hash"));

	INFO("\n - - - - - - - * htable_print * - - - - - - - ");
	htable_print(&htable);

	htable_free(&htable);
	INFO(" --- End of Testing HTable ---");
}

