#ifndef CLIB_SB_H
#define CLIB_SB_H

#include "common.h"

typedef struct StringBuilder {
	isize cap;
	isize len;
	char* data;
} StringBuilder;

StringBuilder sb_new(isize sz);
void          sb_resize(StringBuilder* sb, isize new_sz, bool shrink);
void          sb_append_char(StringBuilder* sb, char c);
void          sb_append_str(StringBuilder* sb, String str);
void          sb_append_cstr(StringBuilder* restrict sb, const char* restrict str);
String        sb_to_string(StringBuilder* sb, Arena* arena);
void          sb_free(StringBuilder* sb);
void          sb_print(const StringBuilder* sb);

#define sb_append(sb, str) _Generic((str), \
		char   : sb_append_char,           \
		int    : sb_append_char,           \
		String : sb_append_str,            \
		char*  : sb_append_cstr,           \
		default: sb_append_cstr            \
	)((sb), (str))

/* -------------------------------------------------------------------- */

#ifdef CLIB_SB_IMPLEMENTATION

StringBuilder sb_new(isize sz)
{
	ASSERT(sz, > 0);

	StringBuilder sb = {
		.cap  = sz,
		.len  = 0,
		.data = smalloc(sz),
	};
	sb.data[0] = '\0';

	return sb;
}

void sb_resize(StringBuilder* sb, isize new_sz, bool shrink)
{
	if (sb->cap < new_sz || (sb->cap != new_sz && shrink)) {
		sb->cap  = MAX(sb->cap*CLIB_SB_RESIZE_FACTOR, new_sz);
		sb->data = srealloc(sb->data, sb->cap);
		CLIB_INFO(TERM_BLUE "[CLIB] Resized StringBuilder [cap=%ld; len=%ld]", sb->cap, sb->len);
	}
}

void sb_append_char(StringBuilder* sb, char c)
{
	sb_resize(sb, sb->len + 1, false);
	sb->data[sb->len++] = c;
	sb->data[sb->len]   = '\0';
}

void sb_append_str(StringBuilder* sb, String str)
{
	sb_resize(sb, sb->len + str.len, false);
	strcpy(sb->data + sb->len, str.data);
	sb->len += str.len;
}

void sb_append_cstr(StringBuilder* restrict sb, const char* restrict str)
{
	isize len = strlen(str);
	sb_resize(sb, sb->len + len, false);
	strcpy(sb->data + sb->len, str);
	sb->len += len;
}

/* If `arena` is NULL, the string builder's memory is used, otherwise it is copied to the arena and free'd */
String sb_to_string(StringBuilder* sb, Arena* arena)
{
	if (!arena) {
		sb_resize(sb, sb->len + 1, true);

		return (String){
			.data = sb->data,
			.len = sb->len
		};
	} else {
		String str = string_new(sb->data, sb->len, arena);
		sb_free(sb);

		return str;
	}
}

void sb_reset(StringBuilder* sb)
{
	sb->len     = 0;
	sb->data[0] = '\0';
}

void sb_free(struct StringBuilder* sb)
{
	sfree(sb->data);
	*sb = (StringBuilder){ 0 };
}

void sb_print(const StringBuilder* sb)
{
	fprintf(DEBUG_OUTPUT, TERM_BLUE "\"%s\" (%ld/%ld)\n" TERM_NORMAL, sb->data, sb->len, sb->cap);
}

#endif /* CLIB_SB_IMPLEMENTATION */
#endif /* CLIB_SB_H */

