#ifndef CLIB_STRING_H
#define CLIB_STRING_H

#include "common.h"

typedef struct String {
	isize len;
	char* data;
} String;

#define STRING(x) (String){ .data = (x), .len = strlen(x), }

String string_new(const char* restrict cstr, isize len, struct Arena* restrict alloc);
String string_cat(isize strc, String* strs, String sep, struct Arena* alloc);
String string_new_split(const char* restrict cstr, char sep, isize index, struct Arena* restrict alloc);
String string_copy(String str, struct Arena* arena);

int string_remove(String str, char c);

bool string_contains(String str, char c);
bool string_starts_with(String str, String start);
bool string_ends_with(String str, String end);

void string_clear(String* str);
void string_free(String* str);

/* -------------------------------------------------------------------- */

#ifdef CLIB_STRING_IMPLEMENTATION

/* Length is calculated if it is <= 0 */
String string_new(const char* restrict cstr, isize len, struct Arena* restrict alloc)
{
	len = len > 0? len: (isize)strlen(cstr);
	String str = {
		.data = alloc? arena_alloc(alloc, len + 1): smalloc(len + 1),
		.len  = len,
	};
	strncpy(str.data, cstr, len + 1);

	return str;
}

String string_cat(isize strc, String* strs, String sep, struct Arena* alloc)
{
	ASSERT(strc, > 0);
	ASSERT(strs, != NULL);

	isize len = (strc - 1) * sep.len;
	for (int i = 0; i < strc; i++)
		len += strs[i].len;

	String str = {
		.data = alloc? arena_alloc(alloc, len + 1): smalloc(len + 1),
		.len  = len,
	};

	char* head = str.data;
	for (isize i = 0; i < strc; i++) {
		memcpy(head, strs[i].data, strs[i].len*sizeof(char));
		head += strs[i].len;
		if (i != strc - 1) {
			memcpy(head, sep.data, sep.len*sizeof(char));
			head += sep.len;
		}
	}
	*head = '\0';

	return str;
}

/* Create a new string by splitting up `cstr` on `sep`s and using `index`.
 *   - If `index` is -1, the last part of the string is given.
 */
String string_new_split(const char* restrict cstr, char sep, isize index, struct Arena* restrict alloc)
{
	index = index == -1? INT64_MAX: index;

	const char* start = cstr;
	int sepc = 0;
	int len  = 0;
	while (*cstr) {
		if (*cstr == sep) {
			sepc++;
			start = ++cstr;
			if (*start == sep)
				start++;
			if (sepc > index)
				return (String){ 0 };
		}
		if (sepc == index) {
			while (*cstr && *cstr++ != sep)
				len++;
			return len? string_new(start, len, alloc): (String){ 0 };
		}
		cstr++;
	}

	return string_new(start, 0, alloc);
}

String string_copy(String str, struct Arena* alloc)
{
	return string_new(str.data, str.len, alloc);
}

bool string_contains(String str, char c)
{
	return strchr(str.data, c);
}

bool string_starts_with(String str, String start)
{
	return !strncmp(str.data, start.data, start.len);
}

bool string_ends_with(String str, String ext)
{
	return !strncmp(str.data + str.len - ext.len, ext.data, ext.len);
}

void string_clear(String* str)
{
	str->len     = 0;
	str->data[0] = '\0';
}

/* Remove all characters `c` from `str`
 *   - Returns the number of characters removed
 */
int string_remove(String str, char c)
{
	int i, j = 0;
	for (i = 0; i < str.len; i++)
		if (str.data[i] != c)
			str.data[j++] = str.data[i];

	int count = i - j;
	while (j < i)
		str.data[j++] = '\0';

	return count;
}

void string_free(String* str)
{
	sfree(str->data);
	str->data = NULL;
	str->len  = 0;
}

#endif /* CLIB_STRING_IMPLEMENTATION */
#endif /* CLIB_STRING_H */

