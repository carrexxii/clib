#include "common.h"

/* [min, max] */
static inline int random_int(int min, int max)
{
    int num;
    int div = RAND_MAX / (max + 1);
    do {
        num = rand() / div;
    } while (!BETWEEN(num, min, max));

    return num;
}

