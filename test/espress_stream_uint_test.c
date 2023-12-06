#include <assert.h>
#include <stdint.h>
#include <byteswap.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"

#if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#	define h_to_l_16(n) (n)
#	define l_to_h_16(n) (n)
#	define h_to_b_16(n) bswap_16(n)
#	define b_to_h_16(n) bswap_16(n)
#	define h_to_l_32(n) (n)
#	define l_to_h_32(n) (n)
#	define h_to_b_32(n) bswap_32(n)
#	define b_to_h_32(n) bswap_32(n)
#	define h_to_l_64(n) (n)
#	define l_to_h_64(n) (n)
#	define h_to_b_64(n) bswap_64(n)
#	define b_to_h_64(n) bswap_64(n)
#else
#	define h_to_l_16(n) bswap_16(n)
#	define l_to_h_16(n) bswap_16(n)
#	define h_to_b_16(n) (n)
#	define b_to_h_16(n) (n)
#	define h_to_l_32(n) bswap_32(n)
#	define l_to_h_32(n) bswap_32(n)
#	define h_to_b_32(n) (n)
#	define b_to_h_32(n) (n)
#	define h_to_l_64(n) bswap_64(n)
#	define l_to_h_64(n) bswap_64(n)
#	define h_to_b_64(n) (n)
#	define b_to_h_64(n) (n)
#endif

#if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#	define h_to_n_16(n) h_to_l_16(n)
#	define n_to_h_16(n) l_to_h_16(n)
#	define h_to_n_32(n) h_to_l_32(n)
#	define n_to_h_32(n) l_to_h_32(n)
#	define h_to_n_64(n) h_to_l_64(n)
#	define n_to_h_64(n) l_to_h_64(n)
#else
#	define h_to_n_16(n) h_to_b_16(n)
#	define n_to_h_16(n) b_to_h_16(n)
#	define h_to_n_32(n) h_to_b_32(n)
#	define n_to_h_32(n) b_to_h_32(n)
#	define h_to_n_64(n) h_to_b_64(n)
#	define n_to_h_64(n) b_to_h_64(n)
#endif

static void _uint16_test();
static void _uint32_test();
static void _uint64_test();

void espress_stream_uint_test()
{
	_uint16_test();
	_uint32_test();
	_uint64_test();
}

static void _uint16_test()
{
	i_t i;
	u_t j;
	struct espress_stream_uint16 u[1];
	struct espress_stream_uint16 v[1];
	uint16_t n = 0;

	espress_stream_uint16_set(u, 0x12, 0x34);
	espress_stream_uint16_get(u, (b_t *)&n, (b_t *)&n + 1);
	assert(0x1234 == b_to_h_16(n));

	espress_stream_uint16_set_u(u, ~(u_t)0);
	espress_stream_uint16_get_u(u, &j);
#	if 2 < U_T_WIDTH
		assert((u_t)(~(u_t)0 & 0xffff) == j);
#	else
		assert((u_t)~(u_t)0 == j);
#	endif

	espress_stream_uint16_set(u, 0x12, 0x34);
	assert(0x1234 == *(uint16_t *)u);

	espress_stream_uint16_set(v, 0xed, 0xcb);
	i = espress_stream_uint16_add(u, v);
	assert(0 == i);
	assert(0xffff == *(uint16_t *)u);
	espress_stream_uint16_set_u(v, 1);
	i = espress_stream_uint16_add(u, v);
	assert(0 != i);
	assert(0 == *(uint16_t *)u);

	i = espress_stream_uint16_sub(u, v);
	assert(0 != i);
	assert(0xffff == *(uint16_t *)u);
	espress_stream_uint16_set(v, 0xed, 0xcb);
	i = espress_stream_uint16_sub(u, v);
	assert(0 == i);
	assert(0x1234 == *(uint16_t *)u);

	espress_stream_uint16_mul(u, v);
	assert(((0x1234 * 0xedcb) & 0xffff) == *(uint16_t *)u);
	espress_stream_uint16_div(u, v);
	assert((((0x1234 * 0xedcb) & 0xffff) / 0xedcb) == *(uint16_t *)u);

	espress_stream_uint16_set(u, 0x12, 0x34);
	espress_stream_uint16_and(u, v);
	assert((0x1234 & 0xedcb) == *(uint16_t *)u);
	espress_stream_uint16_or(u, v);
	assert(((0x1234 & 0xedcb) | 0xedcb) == *(uint16_t *)u);
	espress_stream_uint16_xor(u, v);
	assert((((0x1234 & 0xedcb) | 0xedcb) ^ 0xedcb) == *(uint16_t *)u);
	espress_stream_uint16_not(u);
	assert((~(((0x1234 & 0xedcb) | 0xedcb) ^ 0xedcb) & 0xffff) == *(uint16_t *)u);

	for (i = -16 ; i <= 16 ; i++)
	{
		espress_stream_uint16_set(u, 0x5a, 0xa5);
		espress_stream_uint16_shift(u, i);
		assert(((0 < i) ? ((0x5aa5 << i) & 0xffff) : ((0x5aa5 >> -i) & 0xffff)) == *(uint16_t *)u);
	}

	for (i = -16 ; i <= 16 ; i++)
	{
		espress_stream_uint16_set(u, 0x5a, 0xa5);
		espress_stream_uint16_rotate(u, i);
		assert(((0 < i) ?
			(((0x5aa5 << i) | (0x5aa5 >> (16 - i))) & 0xffff) :
			(((0x5aa5 >> -i) | (0x5aa5 << (16 + i))) & 0xffff)) == *(uint16_t *)u);
	}

	espress_stream_uint16_set(u, 0x12, 0x34);
	espress_stream_uint16_set(v, 0x56, 0x78);
	i = espress_stream_uint16_eq(u, v);
	assert(!i);
	i = espress_stream_uint16_lt(u, v);
	assert(i);
	i = espress_stream_uint16_le(u, v);
	assert(i);
	espress_stream_uint16_set(u, 0x56, 0x78);
	i = espress_stream_uint16_eq(u, v);
	assert(i);
	i = espress_stream_uint16_lt(u, v);
	assert(!i);
	i = espress_stream_uint16_le(u, v);
	assert(i);
	espress_stream_uint16_set(u, 0x9a, 0xbc);
	i = espress_stream_uint16_eq(u, v);
	assert(!i);
	i = espress_stream_uint16_lt(u, v);
	assert(!i);
	i = espress_stream_uint16_le(u, v);
	assert(!i);

	espress_stream_uint16_set(u, 0x12, 0x34);
	espress_stream_uint16_bswap(u);
	assert(0x3412 == *(uint16_t *)u);
}

static void _uint32_test()
{
	i_t i;
	u_t j;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	uint32_t n = 0;

	espress_stream_uint32_set(u, 0x12, 0x34, 0x56, 0x78);
	espress_stream_uint32_get(u, (b_t *)&n, (b_t *)&n + 1, (b_t *)&n + 2, (b_t *)&n + 3);
	assert(0x12345678 == b_to_h_32(n));

	espress_stream_uint32_set_u(u, ~(u_t)0);
	espress_stream_uint32_get_u(u, &j);
#	if 4 < U_T_WIDTH
		assert((u_t)(~(u_t)0 & 0xffffffff) == j);
#	else
		assert((u_t)~(u_t)0 == j);
#	endif

	espress_stream_uint32_set(u, 0x12, 0x34, 0x56, 0x78);
	assert(0x12345678 == *(uint32_t *)u);

	espress_stream_uint32_set(v, 0xed, 0xcb, 0xa9, 0x87);
	i = espress_stream_uint32_add(u, v);
	assert(0 == i);
	assert(0xffffffff == *(uint32_t *)u);
	espress_stream_uint32_set_u(v, 1);
	i = espress_stream_uint32_add(u, v);
	assert(0 != i);
	assert(0 == *(uint32_t *)u);

	i = espress_stream_uint32_sub(u, v);
	assert(0 != i);
	assert(0xffffffff == *(uint32_t *)u);
	espress_stream_uint32_set(v, 0xed, 0xcb, 0xa9, 0x87);
	i = espress_stream_uint32_sub(u, v);
	assert(0 == i);
	assert(0x12345678 == *(uint32_t *)u);

	espress_stream_uint32_mul(u, v);
	assert(((0x12345678 * 0xedcba987) & 0xffffffff) == *(uint32_t *)u);
	espress_stream_uint32_div(u, v);
	assert((((0x12345678 * 0xedcba987) & 0xffffffff) / 0xedcba987) == *(uint32_t *)u);

	espress_stream_uint32_set(u, 0x12, 0x34, 0x56, 0x78);
	espress_stream_uint32_and(u, v);
	assert((0x12345678 & 0xedcba987) == *(uint32_t *)u);
	espress_stream_uint32_or(u, v);
	assert(((0x12345678 & 0xedcba987) | 0xedcba987) == *(uint32_t *)u);
	espress_stream_uint32_xor(u, v);
	assert((((0x12345678 & 0xedcba987) | 0xedcba987) ^ 0xedcba987) == *(uint32_t *)u);
	espress_stream_uint32_not(u);
	assert((~(((0x12345678 & 0xedcba987) | 0xedcba987) ^ 0xedcba987) & 0xffffffff) == *(uint32_t *)u);

	for (i = -32 ; i <= 32 ; i++)
	{
		espress_stream_uint32_set(u, 0x5a, 0x5a, 0xa5, 0xa5);
		espress_stream_uint32_shift(u, i);
		assert(((0 < i) ? ((0x5a5aa5a5 << i) & 0xffffffff) : ((0x5a5aa5a5 >> -i) & 0xffffffff)) == *(uint32_t *)u);
	}

	for (i = -32 ; i <= 32 ; i++)
	{
		espress_stream_uint32_set(u, 0x5a, 0x5a, 0xa5, 0xa5);
		espress_stream_uint32_rotate(u, i);
		assert(((0 < i) ?
			(((0x5a5aa5a5 << i) | (0x5a5aa5a5 >> (32 - i))) & 0xffffffff) :
			(((0x5a5aa5a5 >> -i) | (0x5a5aa5a5 << (32 + i))) & 0xffffffff)) == *(uint32_t *)u);
	}

	espress_stream_uint32_set(u, 0x12, 0x34, 0x12, 0x34);
	espress_stream_uint32_set(v, 0x56, 0x78, 0x56, 0x78);
	i = espress_stream_uint32_eq(u, v);
	assert(!i);
	i = espress_stream_uint32_lt(u, v);
	assert(i);
	i = espress_stream_uint32_le(u, v);
	assert(i);
	espress_stream_uint32_set(u, 0x56, 0x78, 0x56, 0x78);
	i = espress_stream_uint32_eq(u, v);
	assert(i);
	i = espress_stream_uint32_lt(u, v);
	assert(!i);
	i = espress_stream_uint32_le(u, v);
	assert(i);
	espress_stream_uint32_set(u, 0x9a, 0xbc, 0x12, 0x34);
	i = espress_stream_uint32_eq(u, v);
	assert(!i);
	i = espress_stream_uint32_lt(u, v);
	assert(!i);
	i = espress_stream_uint32_le(u, v);
	assert(!i);

	espress_stream_uint32_set(u, 0x12, 0x34, 0x56, 0x78);
	espress_stream_uint32_bswap(u);
	assert(0x78563412 == *(uint32_t *)u);
}

static void _uint64_test()
{
	i_t i;
	u_t j;
	struct espress_stream_uint64 u[1];
	struct espress_stream_uint64 v[1];
	uint64_t n = 0;

	espress_stream_uint64_set(u, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0);
	espress_stream_uint64_get(u, (b_t *)&n, (b_t *)&n + 1, (b_t *)&n + 2, (b_t *)&n + 3,
		(b_t *)&n + 4, (b_t *)&n + 5, (b_t *)&n + 6, (b_t *)&n + 7);
	assert(0x123456789abcdef0 == b_to_h_64(n));

	espress_stream_uint64_set_u(u, ~(u_t)0);
	espress_stream_uint64_get_u(u, &j);
#	if 8 < U_T_WIDTH
		assert((u_t)(~(u_t)0 & 0xffffffffffffffff) == j);
#	else
		assert((u_t)~(u_t)0 == j);
#	endif

	espress_stream_uint64_set(u, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0);
	assert(0x123456789abcdef0 == *(uint64_t *)u);

	espress_stream_uint64_set(v, 0xed, 0xcb, 0xa9, 0x87, 0x65, 0x43, 0x21, 0x0f);
	i = espress_stream_uint64_add(u, v);
	assert(0 == i);
	assert(0xffffffffffffffff == *(uint64_t *)u);
	espress_stream_uint64_set_u(v, 1);
	i = espress_stream_uint64_add(u, v);
	assert(0 != i);
	assert(0 == *(uint64_t *)u);

	i = espress_stream_uint64_sub(u, v);
	assert(0 != i);
	assert(0xffffffffffffffff == *(uint64_t *)u);
	espress_stream_uint64_set(v, 0xed, 0xcb, 0xa9, 0x87, 0x65, 0x43, 0x21, 0x0f);
	i = espress_stream_uint64_sub(u, v);
	assert(0 == i);
	assert(0x123456789abcdef0 == *(uint64_t *)u);

	espress_stream_uint64_mul(u, v);
	assert(((0x123456789abcdef0 * 0xedcba9876543210f) & 0xffffffffffffffff) == *(uint64_t *)u);
	espress_stream_uint64_div(u, v);
	assert((((0x123456789abcdef0 * 0xedcba9876543210f) & 0xffffffffffffffff) / 0xedcba9876543210f) == *(uint64_t *)u);

	espress_stream_uint64_set(u, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0);
	espress_stream_uint64_and(u, v);
	assert((0x123456789abcdef0 & 0xedcba9876543210f) == *(uint64_t *)u);
	espress_stream_uint64_or(u, v);
	assert(((0x123456789abcdef0 & 0xedcba9876543210f) | 0xedcba9876543210f) == *(uint64_t *)u);
	espress_stream_uint64_xor(u, v);
	assert((((0x123456789abcdef0 & 0xedcba9876543210f) | 0xedcba9876543210f) ^ 0xedcba9876543210f) == *(uint64_t *)u);
	espress_stream_uint64_not(u);
	assert((~(((0x123456789abcdef0 & 0xedcba9876543210f) | 0xedcba9876543210f) ^ 0xedcba9876543210f) & 0xffffffffffffffff) == *(uint64_t *)u);

	for (i = -64 ; i <= 64 ; i++)
	{
		espress_stream_uint64_set(u, 0x5a, 0x5a, 0x5a, 0x5a, 0xa5, 0xa5, 0xa5, 0xa5);
		espress_stream_uint64_shift(u, i);
		assert(((0 < i) ? ((0x5a5a5a5aa5a5a5a5 << i & 0xffffffffffffffff)) : ((0x5a5a5a5aa5a5a5a5 >> -i & 0xffffffffffffffff))) == *(uint64_t *)u);
	}

	for (i = -64 ; i <= 64 ; i++)
	{
		espress_stream_uint64_set(u, 0x5a, 0x5a, 0x5a, 0x5a, 0xa5, 0xa5, 0xa5, 0xa5);
		espress_stream_uint64_rotate(u, i);
		assert(((0 < i) ?
			(((0x5a5a5a5aa5a5a5a5 << i) | (0x5a5a5a5aa5a5a5a5 >> (64 - i))) & 0xffffffffffffffff) :
			(((0x5a5a5a5aa5a5a5a5 >> -i) | (0x5a5a5a5aa5a5a5a5 << (64 + i))) & 0xffffffffffffffff)) == *(uint64_t *)u);
	}

	espress_stream_uint64_set(u, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78);
	espress_stream_uint64_set(v, 0x9a, 0xbc, 0xde, 0xf0, 0x9a, 0xbc, 0xde, 0xf0);
	i = espress_stream_uint64_eq(u, v);
	assert(!i);
	i = espress_stream_uint64_lt(u, v);
	assert(i);
	i = espress_stream_uint64_le(u, v);
	assert(i);
	espress_stream_uint64_set(u, 0x9a, 0xbc, 0xde, 0xf0, 0x9a, 0xbc, 0xde, 0xf0);
	i = espress_stream_uint64_eq(u, v);
	assert(i);
	i = espress_stream_uint64_lt(u, v);
	assert(!i);
	i = espress_stream_uint64_le(u, v);
	assert(i);
	espress_stream_uint64_set(u, 0xf0, 0xde, 0xbc, 0x9a, 0xf0, 0xde, 0xbc, 0x9a);
	i = espress_stream_uint64_eq(u, v);
	assert(!i);
	i = espress_stream_uint64_lt(u, v);
	assert(!i);
	i = espress_stream_uint64_le(u, v);
	assert(!i);

	espress_stream_uint64_set(u, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0);
	espress_stream_uint64_bswap(u);
	assert(0xf0debc9a78563412 == *(uint64_t *)u);
}
