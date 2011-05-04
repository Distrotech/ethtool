#ifndef ETHTOOL_BITOPS_H__
#define ETHTOOL_BITOPS_H__

#define BITS_PER_BYTE		8
#define BITS_PER_LONG		(BITS_PER_BYTE * sizeof(long))
#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)

static inline void set_bit(int nr, unsigned long *addr)
{
	addr[nr / BITS_PER_LONG] |= 1UL << (nr % BITS_PER_LONG);
}

static inline void clear_bit(int nr, unsigned long *addr)
{
	addr[nr / BITS_PER_LONG] &= ~(1UL << (nr % BITS_PER_LONG));
}

static inline int test_bit(unsigned int nr, const unsigned long *addr)
{
	return !!((1UL << (nr % BITS_PER_LONG)) &
		  (((unsigned long *)addr)[nr / BITS_PER_LONG]));
}

#endif
