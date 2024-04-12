#ifndef CLIB_MEMORY_H
#define CLIB_MEMORY_H

#include "clib.h"

#if DEBUG
	#define smalloc(sz)       _smalloc((sz),         __FILE__, __LINE__, __func__)
	#define scalloc(n, sz)    _scalloc((n), (sz),    __FILE__, __LINE__, __func__)
	#define srealloc(ptr, sz) _srealloc((ptr), (sz), __FILE__, __LINE__, __func__)
	#define sfree(ptr)        _sfree((ptr),          __FILE__, __LINE__, __func__)
#else
	#define smalloc(sz)       malloc(sz)
	#define scalloc(n, sz)    calloc(n, sz)
	#define srealloc(ptr, sz) realloc(ptr, sz)
	#define sfree(ptr)        free(ptr)
#endif

void* _smalloc(isize sz, const char* file, int line, const char* fn);
void* _scalloc(isize n, isize sz, const char* file, int line, const char* fn);
void* _srealloc(void* mem, isize n, const char* file, int line, const char* fn);
void  _sfree(void* mem, const char* file, int line, const char* fn);

/* -------------------------------------------------------------------- */

#ifdef CLIB_MEMORY_IMPLEMENTATION

/* Fix for asan: https://github.com/google/sanitizers/issues/89 */
int dlclose(void*);
int dlclose(void*) { return 0; }

void* _smalloc(isize s, const char* file, int line, const char* fn)
{
	void* mem = malloc(s);
	if (!mem) {
		ERROR("[CLIB] Malloc failed for %lu bytes", s);
		exit(1);
	} else {
		if (s >= DEBUG_ALLOC_MIN)
			INFO(TERM_BLUE "[CLIB] Allocated %luB (%.2fkB) in \"%s:%d:%s\"", s, (double)s/1024.0, file, line, fn);

		return mem;
	}
}

void* _scalloc(isize n, isize s, const char* file, int line, const char* fn)
{
	isize b = n*s;
	void* mem = calloc(n, s);
	if (!mem) {
		ERROR("[CLIB] Calloc failed for %lu bytes", b);
		exit(1);
	} else {
		if (b >= DEBUG_ALLOC_MIN)
			INFO(TERM_BLUE "[CLIB] Allocated %luB (%.2fkB) in \"%s:%d:%s\"", b, (double)b/1024.0, file, line, fn);

		return mem;
	}
}

void* _srealloc(void* mem, isize s, const char* file, int line, const char* fn)
{
	mem = realloc(mem, s);
	if (!mem) {
		ERROR("[CLIB] Realloc failed for %lu bytes", s);
		exit(1);
	} else {
		if (s >= DEBUG_ALLOC_MIN)
			INFO(TERM_BLUE "[CLIB] Reallocated %luB (%.2fkB) in \"%s:%d:%s\"", s, (double)s/1024.0, file, line, fn);

		return mem;
	}
}

void _sfree(void* mem, const char* file, int line, const char* fn)
{
	if (mem)
		free(mem);
	else
		ERROR("[CLIB] Attempt to free NULL pointer in \"%s:%d:%s\"", file, line, fn);
}

#endif /* CLIB_MEMORY_IMPLEMENTATION */
#endif /* CLIB_MEMORY_H */

