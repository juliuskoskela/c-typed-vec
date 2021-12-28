#ifndef VBUF_H
# define VBUF_H
#include "../typed_vector.h"

typedef struct _point
{
	float x;
	float y;
}	point;

TYPED_VECTOR_HEADER(vbuf, point)
TYPED_VECTOR_HEADER(vals, int)
TYPED_VECTOR_HEADER(rows, vals)

#endif
