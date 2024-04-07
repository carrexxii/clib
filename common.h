#ifndef CLIB_COMMON_H
#define CLIB_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <float.h>
#include <limits.h>
#include <stdalign.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include <fenv.h>
#include <ctype.h>
#include <errno.h>

#if !defined __STDC_NO_ATOMICS__
	#include <stdatomic.h>
#endif
#if !defined __STDC_NO_THREADS__
	#include <threads.h>
#endif

#if defined __unix__ || (defined __APPLE__ && defined __MACH__)
	#include <unistd.h>
#elif defined _WIN32
	#include <io.h>
#endif

typedef  int8_t  int8;
typedef  int16_t int16;
typedef  int32_t int32;
typedef  int64_t int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef unsigned int  uint;
typedef intptr_t      intptr;
typedef uintptr_t     uintptr;
typedef unsigned char byte;
typedef size_t        usize;
typedef ssize_t       isize;

#define atomic _Atomic
typedef thrd_t Thread;
typedef mtx_t  Mutex;
typedef cnd_t  Condition;

enum Axis {
	AXIS_NONE,
	AXIS_X = 1 << 0,
	AXIS_Y = 1 << 1,
	AXIS_Z = 1 << 2,
};
enum Direction {
	DIR_NONE         = 0,
	DIR_UP           = 1 << 0,
	DIR_DOWN         = 1 << 1,
	DIR_RIGHT        = 1 << 2,
	DIR_LEFT         = 1 << 3,
	DIR_FORWARDS     = 1 << 4,
	DIR_BACKWARDS    = 1 << 5,
	DIR_ROTATE_LEFT  = 1 << 6,
	DIR_ROTATE_RIGHT = 1 << 7,
	DIR_N            = 1 << 8,
	DIR_S            = 1 << 9,
	DIR_E            = 1 << 10,
	DIR_W            = 1 << 11,
	DIR_NW           = DIR_N | DIR_W,
	DIR_NE           = DIR_N | DIR_E,
	DIR_SW           = DIR_S | DIR_W,
	DIR_SE           = DIR_S | DIR_E,
	DIR_ALL          = DIR_N | DIR_S | DIR_E | DIR_W,
};

#define PPSTR(x) #x

#define UNPACK2(x) x[0], x[1]
#define UNPACK3(x) x[0], x[1], x[2]
#define UNPACK4(x) x[0], x[1], x[2], x[3]
#define UNPACK5(x) x[0], x[1], x[2], x[3], x[4]
#define UNPACK6(x) x[0], x[1], x[2], x[3], x[4], x[5]
#define UNPACK7(x) x[0], x[1], x[2], x[3], x[4], x[5], x[6]
#define UNPACK8(x) x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]

#ifndef _WIN32
	#define MIN(a, b) ((a) < (b)? (a): (b))
	#define MAX(a, b) ((a) > (b)? (a): (b))
#endif
#define MAX3(a, b, c)    MAX(MAX((a), (b)), (c))
#define BETWEEN(a, b, c) ((bool)((a) >= (b) && (a) <= (c)))
#define CLAMP(a, b, c)   do { ((a) = (a) < (b)? (b): (a) > (c)? (c): (a)); } while (0)
#define ARRAY_SIZE(a)    (sizeof(a) / sizeof(a[0]))
#define DIV_CEIL(a, b)   (((a) + (b) - 1) / (b))
#define AVERAGE(a, b)    (((a) + (b)) / 2)
#define SWAP(a, b) do {   \
		typeof(a) tmp = a; \
		a = b;              \
		b = tmp;             \
	} while (0)
#define DEFAULT(a, b) (a = (a)? (a): (b))

#define STRING_TF(x) ((x)? "true": "false")
#define STRING_YN(x) ((x)? "yes" : "no"   )

#define SELECT1(_1, ...) _1
#define SELECT2(_1, _2, ...) _2
#define SELECT3(_1, _2, _3, ...) _3
#define SELECT4(_1, _2, _3, _4, ...) _4
#define SELECT5(_1, _2, _3, _4, _5, ...) _5
#define SELECT6(_1, _2, _3, _4, _5, _6, ...) _6
#define SELECT7(_1, _2, _3, _4, _5, _6, _7, ...) _7
#define SELECT8(_1, _2, _3, _4, _5, _6, _7, _8, ...) _8

#define FMT_STR(x) _Generic((x),                           \
	char         : "%c"  , signed char           : "%hhd", \
	_Bool        : "%d"  , unsigned char         : "%hhu", \
	short int    : "%hd" , unsigned short int    : "%hu" , \
	int          : "%d"  , unsigned int          : "%u"  , \
	long int     : "%ld" , unsigned long int     : "%lu" , \
	long long int: "%lld", unsigned long long int: "%llu", \
	float        : "%g"  , double                : "%g"  , \
	long double  : "%lg" , char*                 : "%s"  , \
	default: "%p")

#ifndef NO_TERM_COLOUR
#define TERM_NORMAL       "\x1B[0m"
#define TERM_RED          "\x1B[91m"
#define TERM_DARK_RED     "\x1B[31m"
#define TERM_GREEN        "\x1B[92m"
#define TERM_DARK_GREEN   "\x1B[32m"
#define TERM_ORANGE       "\x1B[93m"
#define TERM_DARK_YELLOW  "\x1B[33m"
#define TERM_BLUE         "\x1B[94m"
#define TERM_DARK_BLUE    "\x1B[34m"
#define TERM_MAGENTA      "\x1B[95m"
#define TERM_DARK_MAGENTA "\x1B[35m"
#define TERM_CYAN         "\x1B[96m"
#define TERM_DARK_CYAN    "\x1B[36m"
#define TERM_GRAY         "\x1B[97m"
#define TERM_WHITE        "\x1B[37m"
#else
#define TERM_NORMAL
#define TERM_RED
#define TERM_DARK_RED
#define TERM_GREEN
#define TERM_DARK_GREEN
#define TERM_ORANGE
#define TERM_DARK_YELLOW
#define TERM_BLUE
#define TERM_DARK_BLUE
#define TERM_MAGENTA
#define TERM_DARK_MAGENTA
#define TERM_CYAN
#define TERM_DARK_CYAN
#define TERM_WHITE
#define TERM_GRAY
#endif

#ifdef DEBUG
	#define DEBUG_ALLOC_MIN 1024

	#ifndef DEBUG_OUTPUT
		#define DEBUG_OUTPUT stderr
	#endif
	#define D fprintf(DEBUG_OUTPUT, TERM_RED "*** debug marker ***\n" TERM_NORMAL)
	#define INFO(...) do {                                       \
				fprintf(DEBUG_OUTPUT, __FILE__ ":%d ", __LINE__); \
				fprintf(DEBUG_OUTPUT, __VA_ARGS__);                \
				fprintf(DEBUG_OUTPUT, "\n" TERM_NORMAL);            \
		} while (0)
	#define ERROR(...) do {                                     \
			fprintf(DEBUG_OUTPUT, TERM_RED);                     \
			fprintf(DEBUG_OUTPUT, __VA_ARGS__);                   \
			fprintf(DEBUG_OUTPUT, "\n\t%s:%d in %s\n" TERM_NORMAL, \
					__FILE__, __LINE__, __func__);                  \
		} while (0)
	#define WARN(...) do {                                      \
			fprintf(DEBUG_OUTPUT, TERM_ORANGE);                  \
			fprintf(DEBUG_OUTPUT, __VA_ARGS__);                   \
			fprintf(DEBUG_OUTPUT, "\n\t%s:%d in %s\n" TERM_NORMAL, \
					__FILE__, __LINE__, __func__);                  \
		} while (0)
	#define DV(x) do {                                                                             \
			fprintf(DEBUG_OUTPUT, __FILE__ ":%d ", __LINE__);                                      \
			fprintf(DEBUG_OUTPUT, _Generic((x),                                                    \
				char         : "Char: \"%c\"\n"    , signed char           : "SChar: \"%hhd\"\n" , \
				_Bool        : "Bool: %d\n"        , unsigned char         : "UChar: \"%hhu\"\n" , \
				short int    : "Short: %hd\n"      , unsigned short int    : "UShort: %hu\n"     , \
				int          : "Int: %d\n"         , unsigned int          : "UInt: %u\n"        , \
				long int     : "Long: %ld\n"       , unsigned long int     : "ULong: %lu\n"      , \
				long long int: "Long Long: %lld\n" , unsigned long long int: "ULong Long: %llu\n", \
				float        : "Float: %g\n"       , double                : "Double: %g\n"      , \
				long double  : "Long Double: %lg\n", char*                 : "Char*: \"%s\"\n"   , \
				default: "<unknown or pointer type>%p\n"), (x));                                   \
		} while (0)
	#define ASSERT(v, t) do {   \
		if (!(v t)) {            \
			ERROR(FMT_STR(v), v); \
			assert(v t);           \
		}                           \
	} while (0)
#else
	#define INFO(...)
	#define WARN(...)
	#define ERROR(...)
	#define DV(x)
	#define ASSERT(v, t)
#endif

#ifndef NO_CLIB_INFO
	#define CLIB_INFO(...) INFO(__VA_ARGS__)
#else
	#define CLIB_INFO(...)
#endif
#ifndef NO_CLIB_WARN
	#define CLIB_WARN(...) WARN(__VA_ARGS__)
#else
	#define CLIB_WARN(...)
#endif

/* -------------------------------------------------------------------- */

#ifndef CLIB_HTABLE_SIZE_PER_ELEMENT
	#define CLIB_HTABLE_SIZE_PER_ELEMENT 16
#endif

#ifndef VARRAY_SIZE_MULTIPLIER
	#define VARRAY_SIZE_MULTIPLIER 2
#endif

#ifdef CLIB_IMPLEMENTATION
	#define CLIB_MEMORY_IMPLEMENTATION
	#define CLIB_ARENA_IMPLEMENTATION
	#define CLIB_VARRAY_IMPLEMENTATION
	#define CLIB_IARRAY_IMPLEMENTATION
	#define CLIB_QUEUE_IMPLEMENTATION
	#define CLIB_MINHEAP_IMPLEMENTATION
	#define CLIB_STRING_IMPLEMENTATION
	#define CLIB_FILE_IMPLEMENTATION
	#define CLIB_HTABLE_IMPLEMENTATION
	#define CLIB_RANDOM_IMPLEMENTATION
#endif

#include "memory.h"
#include "arena.h"
#include "varray.h"
#include "iarray.h"
#include "queue.h"
#include "minheap.h"
#include "string.h"
#include "file.h"
#include "htable.h"
#include "random.h"

#endif

