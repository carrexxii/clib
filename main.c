#define CLIB_IMPLEMENTATION
#define NO_CLIB_INFO
#include "clib.h"

// TODO: better testing macros

void test_arena(void);
void test_varray(void);
void test_barray(void);
void test_htable(void);
void test_queue(void);
void test_string(void);

int main()
{
	INFO("------- Starting Tests -------");
	INFO("------------------------------");

	test_arena();
	test_varray();
	test_queue();
	test_htable();
	test_string();
	test_barray();

	INFO("------- Tests Complete -------");
}

