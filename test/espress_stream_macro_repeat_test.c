#include <assert.h>
#include <stdio.h>

#define A(a) _A(a)
#define _A(a) # a

#define FUNC_N(count) static int func_n_ ## count(int *x) {*x = count; return count;}
#define FUNC_N_LIST(count) func_n_ ## count,
#define FUNC_INV_N(count) static int func_inv_n_ ## count(int *x) {*x = -count; return -count;}
#define FUNC_INV_N_LIST(count) func_inv_n_ ## count,

#define ESPRESS_STREAM_MACRO_REPEAT (4)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(FUNC_N)
static int (* const func_n[])(int *x) = {ESPRESS_STREAM_MACRO_REPEAT(FUNC_N_LIST)};
#undef ESPRESS_STREAM_MACRO_REPEAT

#define ESPRESS_STREAM_MACRO_REPEAT (4 + 4)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(FUNC_INV_N)
static int (* const func_inv_n[])(int *x) = {ESPRESS_STREAM_MACRO_REPEAT(FUNC_INV_N_LIST)};
#undef ESPRESS_STREAM_MACRO_REPEAT

void espress_stream_macro_repeat_test()
{
	int i;
	int x;

#	define ESPRESS_STREAM_MACRO_REPEAT (4)
#	include "espress_stream/macro_repeat.h"
	printf("macro_repeat: %s\n", A(ESPRESS_STREAM_MACRO_REPEAT(FUNC_N)));
#	undef ESPRESS_STREAM_MACRO_REPEAT
	for (i = 0 ; i < 4 ; i++)
	{
		assert((i == func_n[i](&x)) && (i == x));
	}

#	define ESPRESS_STREAM_MACRO_REPEAT (4 + 4)
#	include "espress_stream/macro_repeat.h"
	printf("macro_repeat: %s\n", A(ESPRESS_STREAM_MACRO_REPEAT(FUNC_INV_N)));
#	undef ESPRESS_STREAM_MACRO_REPEAT
	for (i = 0 ; i < 8 ; i++)
	{
		assert((-i == func_inv_n[i](&x)) && (-i == x));
	}
}
