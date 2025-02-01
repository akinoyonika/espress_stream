#ifndef __ESPRESS_STREAM_DEPEND_H__
#define __ESPRESS_STREAM_DEPEND_H__

#define _GNU_SOURCE

#include <string.h>
#include <byteswap.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <poll.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/random.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <semaphore.h>
#include <linux/if_alg.h>

#if __ORDER_LITTLE_ENDIAN__ == __BYTE_ORDER__
#	define ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE (1)
#elif __ORDER_BIG_ENDIAN__ == __BYTE_ORDER__
#	define ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE (0)
#endif
#define C_T_WIDTH CHAR_WIDTH
#define B_T_WIDTH UCHAR_WIDTH
#define I_T_WIDTH (4)
#define I_T_MIN (0x80000000)
#define I_T_MAX (0x7fffffff)
#define U_T_WIDTH (4)
#define U_T_MIN (0x00000000)
#define U_T_MAX (0xffffffff)
#if defined(__x86_64)
#	define MAX_I_T_WIDTH (16)
#	define MAX_I_T_MIN (0x80000000000000000000000000000000)
#	define MAX_I_T_MAX (0x7fffffffffffffffffffffffffffffff)
#	define MAX_U_T_WIDTH (16)
#	define MAX_U_T_MIN (0x00000000000000000000000000000000)
#	define MAX_U_T_MAX (0xffffffffffffffffffffffffffffffff)
#else
#	define MAX_I_T_WIDTH (8)
#	define MAX_I_T_MIN (0x8000000000000000)
#	define MAX_I_T_MAX (0x7fffffffffffffff)
#	define MAX_U_T_WIDTH (8)
#	define MAX_U_T_MIN (0x0000000000000000)
#	define MAX_U_T_MAX (0xffffffffffffffff)
#endif
#define c_t char
#define b_t unsigned char
#define i8_t char
#define u8_t unsigned char
#if (2 <= MAX_I_T_WIDTH) && (2 <= MAX_U_T_WIDTH)
#	define i16_t int16_t
#	define u16_t uint16_t
#	define swap_endian_16(n) (*(n) = bswap_16(*(n)))
#endif
#if (4 <= MAX_I_T_WIDTH) && (4 <= MAX_U_T_WIDTH)
#	define i32_t int32_t
#	define u32_t uint32_t
#	define swap_endian_32(n) (*(n) = bswap_32(*(n)))
#endif
#if (8 <= MAX_I_T_WIDTH) && (8 <= MAX_U_T_WIDTH)
#	define i64_t int64_t
#	define u64_t uint64_t
#	define swap_endian_64(n) (*(n) = bswap_64(*(n)))
#endif
#if (16 <= MAX_I_T_WIDTH) && (16 <= MAX_U_T_WIDTH)
#	define bswap_128(n) (((u128_t)bswap_64((u64_t)(n)) << 64) | (u128_t)bswap_64((u64_t)((n) >> 64)))
#	define i128_t __int128_t
#	define u128_t __uint128_t
#	define swap_endian_128(n) (*(n) = bswap_128(*(n)))
#endif
#if (2 == I_T_WIDTH) && (2 == U_T_WIDTH)
#	define i_t int16_t
#	define u_t uint16_t
#elif (4 == I_T_WIDTH) && (4 == U_T_WIDTH)
#	define i_t int32_t
#	define u_t uint32_t
#elif (8 == I_T_WIDTH) && (8 == U_T_WIDTH)
#	define i_t int64_t
#	define u_t uint64_t
#elif (16 == I_T_WIDTH) && (16 == U_T_WIDTH)
#	define i_t __int128_t
#	define u_t __uint128_t
#else
#	error
#endif

#endif /* __ESPRESS_STREAM_DEPEND_H__ */

