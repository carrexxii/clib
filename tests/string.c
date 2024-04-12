#include "../clib.h"

#define BUF_SZ 256
#define RESET  do {             \
		memset(buf1, 0, BUF_SZ); \
		memset(buf2, 0, BUF_SZ);  \
	} while (0)

void test_string()
{
	INFO(TERM_MAGENTA "\n\n --- Testing Strings --- ");

	char buf1[BUF_SZ];
	char buf2[BUF_SZ];
	char* s1 = "Hello, World!";
	char* s2 = "Testing string";
	char* s3 = "z";

	RESET;
	fprintf(DEBUG_OUTPUT, "\n - str_cat -\n");
	fprintf(DEBUG_OUTPUT, "\"%s\" + \"%s\" = ", buf1, s1); str_cat(buf1, s1); fprintf(DEBUG_OUTPUT, "\"%s\"\n", buf1);
	fprintf(DEBUG_OUTPUT, "\"%s\" + \"%s\" = ", buf1, s2); str_cat(buf1, s2); fprintf(DEBUG_OUTPUT, "\"%s\"\n", buf1);
	fprintf(DEBUG_OUTPUT, "\"%s\" + \"%s\" = ", buf1, s3); str_cat(buf1, s3); fprintf(DEBUG_OUTPUT, "\"%s\"\n", buf1);
	fprintf(DEBUG_OUTPUT, "\n");

	RESET;
	fprintf(DEBUG_OUTPUT, "\n - str_split -\n");
	fprintf(DEBUG_OUTPUT, "\"%s\" on ' ' = ", s1); str_split(s1, buf1, buf2, ' '); fprintf(DEBUG_OUTPUT, "\"%s\" + \"%s\"\n", buf1, buf2);
	RESET;
	fprintf(DEBUG_OUTPUT, "\"%s\" on 'l' = ", s1); str_split(s1, buf1, buf2, 'l'); fprintf(DEBUG_OUTPUT, "\"%s\" + \"%s\"\n", buf1, buf2);
	RESET;
	fprintf(DEBUG_OUTPUT, "\"%s\" on '@' = ", s1); str_split(s1, buf1, buf2, '@'); fprintf(DEBUG_OUTPUT, "\"%s\" + \"%s\"\n", buf1, buf2);

	fprintf(DEBUG_OUTPUT, "\n - str_contains -\n");
	fprintf(DEBUG_OUTPUT, "' ' in \"%s\" = %s", s1, STR_TF(str_contains(s1, ' '))); fprintf(DEBUG_OUTPUT, "\t't' in \"%s\" = %s\n", s1, STR_TF(str_contains(s1, 't')));
	fprintf(DEBUG_OUTPUT, "'s' in \"%s\" = %s", s2, STR_TF(str_contains(s2, 's'))); fprintf(DEBUG_OUTPUT, "\t'g' in \"%s\" = %s\n", s2, STR_TF(str_contains(s2, 'g')));
	fprintf(DEBUG_OUTPUT, "'x' in \"%s\" = %s", s3, STR_TF(str_contains(s3, 'x'))); fprintf(DEBUG_OUTPUT, "\t'z' in \"%s\" = %s\n", s3, STR_TF(str_contains(s3, 'z')));

	fprintf(DEBUG_OUTPUT, "\n - str_starts_with -\n");
	fprintf(DEBUG_OUTPUT, "\"%s\" starts with \"%s\" = %s\n", s1, "Hello" , STR_TF(str_starts_with(s1, "Hello")));
	fprintf(DEBUG_OUTPUT, "\"%s\" starts with \"%s\" = %s\n", s2, "string", STR_TF(str_starts_with(s2, "string")));
	fprintf(DEBUG_OUTPUT, "\"%s\" starts with \"%s\" = %s\n", s3, "z"     , STR_TF(str_starts_with(s3, "z")));
	fprintf(DEBUG_OUTPUT, "\"%s\" starts with \"%s\" = %s\n", s3, "x"     , STR_TF(str_starts_with(s3, "x")));

	fprintf(DEBUG_OUTPUT, "\n - str_ends_with -\n");
	fprintf(DEBUG_OUTPUT, "\"%s\" ends with \"%s\" = %s\n", s1, "Hello" , STR_TF(str_ends_with(s1, "Hello")));
	fprintf(DEBUG_OUTPUT, "\"%s\" ends with \"%s\" = %s\n", s2, "string", STR_TF(str_ends_with(s2, "string")));
	fprintf(DEBUG_OUTPUT, "\"%s\" ends with \"%s\" = %s\n", s3, "z"     , STR_TF(str_ends_with(s3, "z")));
	fprintf(DEBUG_OUTPUT, "\"%s\" ends with \"%s\" = %s\n", s3, "x"     , STR_TF(str_ends_with(s3, "x")));

	INFO(TERM_MAGENTA "\n\n --- Testing StringBuilder --- ");
	StringBuilder sb = sb_new(512);
	fprintf(DEBUG_OUTPUT, "sb = "); sb_print(&sb);
	sb_append(&sb, s1);
	fprintf(DEBUG_OUTPUT, "sb + \"%s\" = ", s1); sb_print(&sb);
	sb_append(&sb, s2);
	fprintf(DEBUG_OUTPUT, "sb + \"%s\" = ", s2); sb_print(&sb);
	sb_append(&sb, s3);
	fprintf(DEBUG_OUTPUT, "sb + \"%s\" = ", s3); sb_print(&sb);
	sb_append(&sb, '<');
	fprintf(DEBUG_OUTPUT, "sb + \"<\" = "); sb_print(&sb);
	sb_append(&sb, '?');
	fprintf(DEBUG_OUTPUT, "sb + \"?\" = "); sb_print(&sb);
	sb_free(&sb);

	INFO("\n --- Done Testing Strings --- ");
}

