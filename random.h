#ifndef CLIB_RANDOM_H
#define CLIB_RANDOM_H

#include "clib.h"

int random_int(int min, int max);

/* -------------------------------------------------------------------- */

#ifdef CLIB_RANDOM_IMPLEMENTATION

/* [min, max] */
int random_int(int min, int max)
{
    int num;
    int div = RAND_MAX / (max + 1);
    do {
        num = rand() / div;
    } while (!BETWEEN(num, min, max));

    return num;
}

#endif /* CLIB_RANDOM_IMPLEMENTATION */
#endif /* CLIB_RANDOM_H */

