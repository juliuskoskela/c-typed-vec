#include "vbuf.h"
#include "stdio.h"

void print_point(point p)
{
	printf("%.1f %.1f\n", p.x, p.y);
}

bool filter_zero_dot(point p)
{
	if (p.x * p.y != 0)
		return (true);
	else
		return (false);
}

int cmp_dot(point a, point b)
{
	if (a.x  * a.y > b.x * b.y)
		return (1);
	else if (a.x  * a.y < b.x * b.y)
		return (-1);
	else
		return (0);
}

int main(void)
{
	vbuf a;
	vbuf b;

	vbuf_new(&a, 0);
	vbuf_push(&a, (point) {0.0f, 0.0f});
	vbuf_push(&a, (point) {1.0f, 0.0f});
	vbuf_push(&a, (point) {2.0f, 1.0f});
	vbuf_push(&a, (point) {3.0f, 4.0f});
	vbuf_push(&a, (point) {3.0f, 0.0f});
	vbuf_push(&a, (point) {1.0f, 4.0f});
	vbuf_push(&a, (point) {3.0f, 2.0f});
	vbuf_push(&a, (point) {1.0f, 4.0f});
	vbuf_push(&a, (point) {2.0f, 0.0f});
	vbuf_push(&a, (point) {1.0f, 4.0f});
	vbuf_push(&a, (point) {0.5f, 1.0f});
	vbuf_push(&a, (point) {1.0f, 1.7f});
	printf("\noriginal vector: \n\n");
	vbuf_iter(&a, print_point);
	vbuf_new(&b, 0);
	vbuf_filter(&b, &a, filter_zero_dot);
	printf("\nfilter dot product = 0.0: \n\n");
	vbuf_iter(&b, print_point);
	printf("\nsort by dot product: \n\n");
	vbuf_sort(&b, cmp_dot);
	vbuf_iter(&b, print_point);
	printf("\nsizeof vbuf = %zu\n", vbuf_byte_size(&a));
	vbuf_free(&a);
	vbuf_free(&b);
}
