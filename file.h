#ifndef CLIB_FILE_H
#define CLIB_FILE_H

#include "common.h"

FILE*  file_open(char* restrict path, char* restrict mode);
char*  file_load(char* path);
intptr file_size(FILE* file);
char*  file_loadf(FILE* file);

/* -------------------------------------------------------------------- */

#ifdef CLIB_FILE_IMPLEMENTATION

FILE* file_open(char* restrict path, char* restrict mode)
{
	FILE* file = fopen(path, mode);
	fflush(file);
	if (!file)
		ERROR("[CLIB] Failed to open file: \"%s\"", path);
	else
		CLIB_INFO(TERM_BLUE "[CLIB] Opened file \"%s\"", path);

	return file;
}

char* file_load(char* path)
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

#endif /* CLIB_FILE_IMPLEMENTATION */
#endif /* CLIB_FILE_H */

