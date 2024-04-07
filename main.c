#define CLIB_IMPLEMENTATION
#include "common.h"

// TODO: better testing macros

void test_arena(void);
void test_varray(void);
void test_htable(void);
void test_queue(void);

int main()
{
	INFO("------- Starting Tests -------");
	INFO("------------------------------");

	test_arena();
	test_varray();
	test_htable();
	test_queue();
	// test_string();

	INFO("------- Tests Complete -------");
}

// static void test_string()
// {
// 	INFO(" --- Testing String ---");
// 	char* str = "some/file/path.txt";
// 	INFO("Splitting with `string_new_split()`: \"%s\"", str);
// 	INFO("\t0 = %s", string_new_split(str, '/', 0, NULL).data);
// 	INFO("\t1 = %s", string_new_split(str, '/', 1, NULL).data);
// 	INFO("\t2 = %s", string_new_split(str, '/', 2, NULL).data);
// 	INFO("\t100 = %s", string_new_split(str, '/', 100, NULL).data);
// 	INFO("\t-123 = %s", string_new_split(str, '/', -123, NULL).data);
// 	INFO("\t-1 = %s", string_new_split(str, '/', -1, NULL).data);

// 	str = "abc;123;xyz";
// 	INFO("\nSplitting with `string_new_split()`: \"%s\"", str);
// 	INFO("\t0 = %s", string_new_split(str, ';', 0, NULL).data);
// 	INFO("\t1 = %s", string_new_split(str, ';', 1, NULL).data);
// 	INFO("\t2 = %s", string_new_split(str, ';', 2, NULL).data);
// 	INFO("\t3 = %s", string_new_split(str, ';', 3, NULL).data);
// 	INFO("\t-1 = %s", string_new_split(str, ';', -1, NULL).data);

// 	str = ".b.c.d..f";
// 	INFO("\nSplitting with `string_new_split()`: \"%s\"", str);
// 	INFO("\t0 = %s", string_new_split(str, '.', 0, NULL).data);
// 	INFO("\t1 = %s", string_new_split(str, '.', 1, NULL).data);
// 	INFO("\t2 = %s", string_new_split(str, '.', 2, NULL).data);
// 	INFO("\t3 = %s", string_new_split(str, '.', 3, NULL).data);
// 	INFO("\t4 = %s", string_new_split(str, '.', 4, NULL).data);
// 	INFO("\t5 = %s", string_new_split(str, '.', 5, NULL).data);
// 	INFO("\t15 = %s", string_new_split(str, '.', 15, NULL).data);
// 	INFO("\t-10 = %s", string_new_split(str, '.', -10, NULL).data);
// 	INFO("\t-1 = %s", string_new_split(str, '.', -1, NULL).data);
// }

