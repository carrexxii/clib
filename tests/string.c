#include "../common.h"

#define BUF_SZ 256
#define RESET  do {             \
		memset(buf1, 0, BUF_SZ); \
		memset(buf2, 0, BUF_SZ);  \
	} while (0)

void test_string()
{
	INFO(" --- Testing Strings --- ");

	char buf1[BUF_SZ];
	char buf2[BUF_SZ];
	char* s1 = "Hello, World!";
	char* s2 = "Testing string";
	char* s3 = "z";

	RESET;
	printf("\n - str_cat -\n");
	printf("\"%s\" + \"%s\" = ", buf1, s1); str_cat(buf1, s1); printf("\"%s\"\n", buf1);
	printf("\"%s\" + \"%s\" = ", buf1, s2); str_cat(buf1, s2); printf("\"%s\"\n", buf1);
	printf("\"%s\" + \"%s\" = ", buf1, s3); str_cat(buf1, s3); printf("\"%s\"\n", buf1);
	printf("\n");

	RESET;
	printf("\n - str_split -\n");
	printf("\"%s\" on ' ' = ", s1); str_split(s1, buf1, buf2, ' '); printf("\"%s\" + \"%s\"\n", buf1, buf2);
	RESET;
	printf("\"%s\" on 'l' = ", s1); str_split(s1, buf1, buf2, 'l'); printf("\"%s\" + \"%s\"\n", buf1, buf2);
	RESET;
	printf("\"%s\" on '@' = ", s1); str_split(s1, buf1, buf2, '@'); printf("\"%s\" + \"%s\"\n", buf1, buf2);

	printf("\n - str_contains -\n");
	printf("' ' in \"%s\" = %s", s1, STR_TF(str_contains(s1, ' '))); printf("\t't' in \"%s\" = %s\n", s1, STR_TF(str_contains(s1, 't')));
	printf("'s' in \"%s\" = %s", s2, STR_TF(str_contains(s2, 's'))); printf("\t'g' in \"%s\" = %s\n", s2, STR_TF(str_contains(s2, 'g')));
	printf("'x' in \"%s\" = %s", s3, STR_TF(str_contains(s3, 'x'))); printf("\t'z' in \"%s\" = %s\n", s3, STR_TF(str_contains(s3, 'z')));

	printf("\n - str_starts_with -\n");
	printf("\"%s\" starts with \"%s\" = %s\n", s1, "Hello" , STR_TF(str_starts_with(s1, "Hello")));
	printf("\"%s\" starts with \"%s\" = %s\n", s2, "string", STR_TF(str_starts_with(s2, "string")));
	printf("\"%s\" starts with \"%s\" = %s\n", s3, "z"     , STR_TF(str_starts_with(s3, "z")));
	printf("\"%s\" starts with \"%s\" = %s\n", s3, "x"     , STR_TF(str_starts_with(s3, "x")));

	printf("\n - str_ends_with -\n");
	printf("\"%s\" ends with \"%s\" = %s\n", s1, "Hello" , STR_TF(str_ends_with(s1, "Hello")));
	printf("\"%s\" ends with \"%s\" = %s\n", s2, "string", STR_TF(str_ends_with(s2, "string")));
	printf("\"%s\" ends with \"%s\" = %s\n", s3, "z"     , STR_TF(str_ends_with(s3, "z")));
	printf("\"%s\" ends with \"%s\" = %s\n", s3, "x"     , STR_TF(str_ends_with(s3, "x")));

	INFO(" --- Done Testing Strings --- ");
}

