#include "memswap.h"

static inline void memswap64(uint64_t *a, uint64_t *b)
{
	if (a == b)
		return ;
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static inline void memswap32(uint32_t *a, uint32_t *b)
{
	if (a == b)
		return ;
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static inline void memswap16(uint16_t *a, uint16_t *b)
{
	if (a == b)
		return ;
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static inline void memswap8(uint8_t *a, uint8_t *b)
{
	if (a == b)
		return ;
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static inline void	memswap512(uint64_t *a, uint64_t *b)
{
	memswap64(&a[0], &b[0]);
	memswap64(&a[1], &b[1]);
	memswap64(&a[2], &b[2]);
	memswap64(&a[3], &b[3]);
	memswap64(&a[4], &b[4]);
	memswap64(&a[5], &b[5]);
	memswap64(&a[6], &b[6]);
	memswap64(&a[7], &b[7]);
}

static inline void	swap_small(char *a, char *b, size_t n)
{
	if (n >= 8)
	{
		memswap64((uint64_t *)a, (uint64_t *)b);
		return ;
	}
	if (n >= 4)
	{
		memswap32((uint32_t *)a, (uint32_t *)b);
		a += 4;
		b += 4;
	}
	if (n & 2)
	{
		memswap16((uint16_t *)a, (uint16_t *)b);
		a += 2;
		b += 2;
	}
	if (n & 1)
		memswap8((uint8_t *)a, (uint8_t *)b);
}

static inline void	swap_big(uint64_t *a, uint64_t *b, size_t n)
{
	size_t	chunks;
	size_t	offset;

	chunks = n >> 3;
	offset = n - (chunks << 3);
	while (chunks--)
	{
		memswap512(a, b);
		a += 8;
		b += 8;
	}
	while (offset--)
	{
		memswap64(a, b);
		a += 1;
		b += 1;
	}
}

void	memswap(void *a, void *b, size_t n)
{
	char	*raw_a;
	char	*raw_b;
	size_t	qwords;
	size_t	aligned_size;

	raw_a = a;
	raw_b = b;
	if (raw_a == raw_b)
		return ;
	qwords = n >> 3;
	aligned_size = qwords << 3;
	if (n >= 8)
		swap_big((uint64_t *)a, (uint64_t *)b, qwords);
	aligned_size = qwords << 3;
	n -= aligned_size;
	raw_a += aligned_size;
	raw_b += aligned_size;
	swap_small(raw_a, raw_b, n);
}