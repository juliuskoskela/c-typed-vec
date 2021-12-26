#ifndef MEMSWAP_H
# define MEMSWAP_H

#include "stdint.h"
#include "unistd.h"

void memswap(void *a, void *b, size_t bytes);

#endif
