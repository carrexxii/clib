#ifndef CLIB_STR_H
#define CLIB_STR_H

#include "common.h"

char* str_cat(char* restrict dst, const char* restrict src);
void  str_split(const char* restrict src, char* restrict dst1, char* restrict dst2, char c);
void  str_copy(char* restrict dst, const char* restrict src);

bool str_contains_char(const char* s, char c);
bool str_contains_str(const char* restrict s1, const char* restrict s2);
bool str_starts_with(const char* restrict s, const char* restrict start);
bool str_ends_with(const char* restrict s, const char* restrict end);

#define str_contains(s, x) _Generic((x), \
		char : str_contains_char,        \
		char*: str_contains_str,         \
		default: str_contains_char       \
	)((s), (x))

/* -------------------------------------------------------------------- */

#ifdef CLIB_STR_IMPLEMENTATION

void str_copy(char* restrict dst, const char* restrict src)
{
	strcpy(dst, src);
}

char* str_cat(char* restrict dst, const char* restrict src)
{
	strcat(dst, src);
	return dst;
}

/* Splits on the first occurence of `c`
 *   - The character split on is not copied
 *   - If `c` is not found, the whole string is copied to `dst1` and `dst2` is set to `""`
 */
void str_split(const char* restrict src, char* restrict dst1, char* restrict dst2, char c)
{
	while (*src && *src != c)
		*dst1++ = *src++;
	*dst1 = '\0';

	if (*src) {
		src++;
	} else {
		*dst2 = '\0';
		return;
	}

	while (*src)
		*dst2++ = *src++;
	*dst2 = '\0';
}

bool str_contains_char(const char* s, char c)
{
	return strchr(s, c);
}

bool str_contains_str(const char* restrict s1, const char* restrict s2)
{
	return strstr(s1, s2);
}

bool str_starts_with(const char* restrict s, const char* restrict start)
{
	while (*start)
		if (*s++ != *start++)
			return false;

	return true;
}

bool str_ends_with(const char* restrict s, const char* restrict end)
{
	isize s_len   = strlen(s);
	isize end_len = strlen(end);
	return !strncmp(s + s_len - end_len, end, end_len);
}

#endif /* CLIB_STR_IMPLEMENTATION */
#endif /* CLIB_STR_H */

