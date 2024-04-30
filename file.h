#ifndef CLIB_FILE_H
#define CLIB_FILE_H

#include "clib.h"

FILE*  file_open(const char* restrict path, const char* restrict mode);
char*  file_load(const char* path);
intptr file_size(FILE* file);
char*  file_loadf(FILE* file);
void   file_extension(const char* restrict path, char* restrict fname, char* restrict ext);

/* -------------------------------------------------------------------- */

#ifdef CLIB_FILE_IMPLEMENTATION

FILE* file_open(const char* restrict path, const char* restrict mode)
{
	FILE* file = fopen(path, mode);
	fflush(file);
	if (!file)
		ERROR("[CLIB] Failed to open file: \"%s\"", path);
	else
		CLIB_INFO(TERM_BLUE "[CLIB] Opened file \"%s\"", path);

	return file;
}

char* file_load(const char* path)
{
	return file_loadf(file_open(path, "rb"));
}

intptr file_size(FILE* file)
{
#if defined _WIN32
	isize sz = _filelengthi64(fileno(file));
#elif defined _POSIX_VERSION
	fseek(file, 0, SEEK_END);
	isize sz = ftell(file)*sizeof(char);
	rewind(file);
#endif

	return sz;
}

char* file_loadf(FILE* file)
{
	intptr sz = file_size(file);
	char* buf = smalloc(sz + 1);
	fread(buf, 1, sz, file);
	buf[sz] = '\0';

	fclose(file);
	return buf;
}

void file_extension(const char* restrict path, char* restrict fname, char* restrict ext)
{
	while (*++path);
	while (*--path != '/');
	if (fname) {
		while ((*fname++ = *++path) != '.');
		*--fname = '\0'; /* Remove the period */
	} else {
		while (*++path != '.');
	}
	while ((*ext++ = *++path));
}

#endif /* CLIB_FILE_IMPLEMENTATION */
#endif /* CLIB_FILE_H */

