#ifndef BASE_H
#define BASE_H

///////////////////
// useful includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"

////////////////////
// custom assertion
#define ASSERT(c,...)\
    do {\
        if (!(c)) {\
            fprintf(stderr, "assertion failure at %s:%d: ", __FILE__, __LINE__);\
            fprintf(stderr, ##__VA_ARGS__);\
            abort();\
        }\
    } while (0)

#endif // BASE_H
