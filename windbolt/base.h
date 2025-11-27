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
#define ASSERT(c,msg) if(!(c)){ fprintf(stderr, "assertion failure at %s:%i: %s\n", __FILE__, __LINE__, (msg)); abort(); }

#endif // BASE_H
