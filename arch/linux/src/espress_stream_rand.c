#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/rand.h"

static i_t _initialize(void);
static void _finalize(void);
static void _get(b_t *buffer, u_t size);

struct espress_stream_rand_ops const espress_stream_rand[] =
{{
	_initialize,
	_finalize,
	_get
}};

static i_t _initialize(void)
{
	return 0;
}

static void _finalize(void)
{
}

static void _get(b_t *buffer, u_t size)
{
	ssize_t s;
	while (0 < size)
	{
		s = getrandom(buffer, size, 0);
		if (0 < s)
		{
			buffer += s;
			size -= s;
		}
	}
}
