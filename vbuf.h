#include "typed_vector.h"
#include "memswap/memswap.h"
#include "stdio.h"

typedef struct _point
{
	float x;
	float y;
}	point;

TYPED_VECTOR_HEADER(vbuf, point)
